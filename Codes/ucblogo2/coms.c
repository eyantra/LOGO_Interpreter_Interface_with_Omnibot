/*
 *      coms.c	  program execution control module	dvb
 *
 *	Copyright (C) 1993 by the Regents of the University of California
 *
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

#define WANT_EVAL_REGS 1
#include "logo.h"
#include "globals.h"
#include <math.h>
#include <unistd.h>

#include <termio.h>

NODE *make_cont(enum labels cont, NODE *val) {
	NODE *retval = cons(NIL, val);
	retval->n_car = (NODE *) cont;
	settype(retval, CONT);
	return retval;
}

NODE *loutput(NODE *arg) {
	if (NOT_THROWING) {
		if (ufun == NIL) {
			err_logo(AT_TOPLEVEL, fun);
		} else if (val_status & OUTPUT_TAIL
		/* This happens if OP seen when val_status & STOP_OK */
		|| !(val_status & OUTPUT_OK)) {
			/* This happens on OP OP 3 */
			if (didnt_output_name == NIL)
				didnt_output_name = fun;
			if (didnt_get_output == UNBOUND) {
				didnt_get_output = cons_list(0, theName(Name_output), ufun,
						this_line, END_OF_LIST);
				/* Not quite right; could be .maybeoutput */
				didnt_output_name = fun;
			}
			err_logo(DIDNT_OUTPUT, NIL);
		} else {
			stopping_flag = OUTPUT;
			output_unode = current_unode;
			output_node = car(arg);
		}
	}
	return (UNBOUND);
}

NODE *lstop(NODE *args) {
	if (NOT_THROWING) {
		if (ufun == NIL)
			err_logo(AT_TOPLEVEL, theName(Name_stop));
		else if (val_status & OUTPUT_TAIL || !(val_status & STOP_OK)) {
			didnt_output_name = fun;
			err_logo(DIDNT_OUTPUT, NIL);
		} else {
			stopping_flag = STOP;
			output_unode = current_unode;
		}
	}
	return (UNBOUND);
}

NODE *lthrow(NODE *arg) {
	if (NOT_THROWING) {
		if (isName(car(arg), Name_error)) {
			if (cdr(arg) != NIL)
				err_logo(USER_ERR_MESSAGE, cadr(arg));
			else
				err_logo(USER_ERR, UNBOUND);
		} else {
			stopping_flag = THROWING;
			throw_node = car(arg);
			if (cdr(arg) != NIL)
				output_node = cadr(arg);
			else
				output_node = UNBOUND;
		}
	}
	return (UNBOUND);
}

NODE *lcatch(NODE *args) {
	return make_cont(catch_continuation, cons(car(args), lrun(cdr(args))));
}

extern NODE *evaluator(NODE *list, enum labels where);

int torf_arg(NODE *args) {
	NODE *arg = car(args);

	while (NOT_THROWING) {
		if (is_list(arg)) { /* accept a list and run it */
			val_status = VALUE_OK;
			arg = evaluator(arg, begin_seq);
		}
		if (isName(arg, Name_true))
			return TRUE;
		if (isName(arg, Name_false))
			return FALSE;
		if (NOT_THROWING)
			arg = err_logo(BAD_DATA, arg);
	}
	return -1;
}

NODE *lgoto(NODE *args) {
	return make_cont(goto_continuation, car(args));
}

NODE *ltag(NODE *args) {
	return UNBOUND;
}

NODE *lnot(NODE *args) {
	int arg = torf_arg(args);

	if (NOT_THROWING) {
		if (arg)
			return (FalseName());
		else
			return (TrueName());
	}
	return (UNBOUND);
}

NODE *land(NODE *args) {
	int arg;

	if (args == NIL)
		return (TrueName());
	while (NOT_THROWING) {
		arg = torf_arg(args);
		if (arg == FALSE)
			return (FalseName());
		args = cdr(args);
		if (args == NIL)
			break;
	}
	if (NOT_THROWING)
		return (TrueName());
	else
		return (UNBOUND);
}

NODE *lor(NODE *args) {
	int arg;

	if (args == NIL)
		return (FalseName());
	while (NOT_THROWING) {
		arg = torf_arg(args);
		if (arg == TRUE)
			return (TrueName());
		args = cdr(args);
		if (args == NIL)
			break;
	}
	if (NOT_THROWING)
		return (FalseName());
	else
		return (UNBOUND);
}

NODE *runnable_arg(NODE *args) {
	NODE *arg = car(args);

	if (!aggregate(arg)) {
		setcar(args, parser(arg, TRUE));
		arg = car(args);
	}
	while (!is_list(arg) && NOT_THROWING) {
		setcar(args, err_logo(BAD_DATA, arg));
		arg = car(args);
	}
	return (arg);
}

NODE *lif(NODE *args) { /* macroized */
	NODE *yes;
	int pred;

	if (cddr(args) != NIL)
		return (lifelse(args));

	pred = torf_arg(args);
	yes = runnable_arg(cdr(args));
	if (NOT_THROWING) {
		if (pred)
			return (yes);
		return (NIL);
	}
	return (UNBOUND);
}

NODE *lifelse(NODE *args) { /* macroized */
	NODE *yes, *no;
	int pred;

	pred = torf_arg(args);
	yes = runnable_arg(cdr(args));
	no = runnable_arg(cddr(args));
	if (NOT_THROWING) {
		if (pred)
			return (yes);
		return (no);
	}
	return (UNBOUND);
}

NODE *lrun(NODE *args) { /* macroized */
	NODE *arg = runnable_arg(args);

	if (NOT_THROWING)
		return (arg);
	return (UNBOUND);
}

NODE *lrunresult(NODE *args) {
	return make_cont(runresult_continuation, lrun(args));
}

NODE *pos_int_arg(NODE *args) {
	NODE *arg = car(args), *val;
	FIXNUM i;
	FLONUM f;

	val = cnv_node_to_numnode(arg);
	while ((nodetype(val) != INT || getint(val) < 0) && NOT_THROWING) {
		if (nodetype(val) == FLOATT && fmod((f = getfloat(val)), 1.0) == 0.0
				&& f >= 0.0 && f < (FLONUM) MAXLOGOINT) {
			i = (FIXNUM) f;
			val = make_intnode(i);
			break;
		}
		setcar(args, err_logo(BAD_DATA, arg));
		arg = car(args);
		val = cnv_node_to_numnode(arg);
	}
	setcar(args, val);
	if (nodetype(val) == INT)
		return (val);
	return UNBOUND;
}

NODE *lrepeat(NODE *args) {
	NODE *cnt, *torpt, *retval = NIL;

	cnt = pos_int_arg(args);
	torpt = lrun(cdr(args));
	if (NOT_THROWING) {
		retval = make_cont(repeat_continuation, cons(cnt, torpt));
	}
	return (retval);
}

NODE *lrepcount(NODE *args) {
	return make_intnode(user_repcount);
}

NODE *lforever(NODE *args) {
	NODE *torpt = lrun(args);

	if (NOT_THROWING)
		return make_cont(repeat_continuation, cons(make_intnode(-1), torpt));
	return NIL;
}

NODE *ltest(NODE *args) {
	int arg = torf_arg(args);

	if (ufun != NIL && tailcall != 0)
		return UNBOUND;
	if (NOT_THROWING) {
		dont_fix_ift = arg + 1;
	}
	return (UNBOUND);
}

NODE *liftrue(NODE *args) {
	if (ift_iff_flag < 0)
		return (err_logo(NO_TEST, NIL));
	else if (ift_iff_flag > 0)
		return (lrun(args));
	else
		return (NIL);
}

NODE *liffalse(NODE *args) {
	if (ift_iff_flag < 0)
		return (err_logo(NO_TEST, NIL));
	else if (ift_iff_flag == 0)
		return (lrun(args));
	else
		return (NIL);
}

void prepare_to_exit(BOOLEAN okay) {

	char ef[30];

	charmode_off();
	sprintf(ef, "/tmp/logo%d", (int) getpid());
	unlink(ef);
}

NODE *lbye(NODE *args) {
	prepare_to_exit(TRUE);
	if (ufun != NIL || loadstream != stdin)
		exit(0);
	if (isatty(0) && isatty(1))
		lcleartext(NIL);
	ndprintf(stdout, "%t\n", message_texts[THANK_YOU]);
	ndprintf(stdout, "%t\n", message_texts[NICE_DAY]);

	exit(0);
	return UNBOUND; /* not reached, but makes compiler happy */
}

NODE *lwait(NODE *args) {
	NODE *num;
	unsigned int n;
	unsigned int seconds, microseconds;

	num = pos_int_arg(args);
	if (NOT_THROWING) {
		/*#ifdef HAVE_WX
		 n = (unsigned int)getint(num) * 10; // milliseconds
		 wxLogoSleep(n);
		 return(UNBOUND);
		 #endif*/
		fflush(stdout); /* csls v. 1 p. 7 */

		fix_turtle_shownness();

		if (getint(num) > 0) {
			n = (unsigned int) getint(num);

			if (seconds = n / 60)
				sleep(seconds);

			if (microseconds = (n % 60) * 16667)
				usleep(microseconds);
		}
	}
	return (UNBOUND);
}

NODE *lshell(NODE *args) {
	extern FILE *popen();
	char cmdbuf[300];
	FILE *strm;
	NODE *head = NIL, *tail = NIL, *this;
	BOOLEAN wordmode = FALSE;
	int len;
	char *old_stringptr = print_stringptr;
	int old_stringlen = print_stringlen;

	if (cdr(args) != NIL)
		wordmode = TRUE;
	print_stringptr = cmdbuf;
	print_stringlen = 300;
	ndprintf((FILE *) NULL, "%p\n", car(args));
	*print_stringptr = '\0';
	strm = popen(cmdbuf, "r");
	print_stringptr = old_stringptr;
	print_stringlen = old_stringlen;
	fgets(cmdbuf, 300, strm);
	while (!feof(strm)) {
		len = (int) strlen(cmdbuf);
		if (cmdbuf[len - 1] == '\n')
			cmdbuf[--len] = '\0';
		if (wordmode)
			this = make_strnode(cmdbuf, (struct string_block *) NULL, len,
					STRING, strnzcpy);
		else
			this = parser(make_static_strnode(cmdbuf), FALSE);
		if (head == NIL) {
			tail = head = cons(this, NIL);
		} else {
			setcdr(tail, cons(this, NIL));
			tail = cdr(tail);
		}
		fgets(cmdbuf, 300, strm);
	}
	pclose(strm);
	return (head);
}
