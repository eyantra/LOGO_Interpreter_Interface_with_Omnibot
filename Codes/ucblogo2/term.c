/*
 *	term.c		terminal cursor control			dvb
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

#include "logo.h"
#include "globals.h"

#include <unistd.h>

#include <termio.h>

#undef TRUE
#undef FALSE

#undef TRUE
#undef FALSE

#define FALSE	0
#define TRUE	1

int x_coord, y_coord, x_max, y_max;

char PC;
char *BC;
char *UP;
short ospeed;
char bp[1024];
char cl_arr[40];
char cm_arr[40];
char so_arr[40];
char se_arr[40];

struct termio tty_cooked, tty_cbreak;

int interactive, tty_charmode;

extern char **environ, *tgoto(), *tgetstr();

char *termcap_ptr;

int termcap_putter(char ch) {
	*termcap_ptr++ = ch;
	return 0;
}

void termcap_getter(char *cap, char *buf) {
	char temp[40];
	char *str;
	char *temp_ptr = temp;

	termcap_ptr = buf;
	str = tgetstr(cap, &temp_ptr);
	/* if (str == NULL) str = temp; */
	tputs(str, 1, termcap_putter);
}

void term_init(void) {
	char *emacs; /* emacs change */
	int term_sg;

	interactive = isatty(0);

	if (interactive) {
		ioctl(0, TCGETA, (char *) (&tty_cooked));
		tty_cbreak = tty_cooked;
		tty_cbreak.c_cc[VMIN] = '\01';
		tty_cbreak.c_cc[VTIME] = '\0';
		tty_cbreak.c_lflag &= ~(ECHO | ICANON);
	}
	tty_charmode = 0;
	tgetent(bp, getenv("TERM"));

	/* emacs changes */

	emacs = getenv("EMACS");

	/* check if started from emacs */
	if (!emacs || *emacs != 't') { /* read from termcap */
		x_max = tgetnum("co");
		y_max = tgetnum("li");
	} else { /* read environment variables */
		emacs = getenv("COLUMNS");
		if (!emacs)
			x_max = 0;
		else
			x_max = atoi(emacs);
		emacs = getenv("LINES");
		if (!emacs)
			y_max = 0;
		else
			y_max = atoi(emacs);
	}

	/* end emacs changes */

	if (x_max <= 0)
		x_max = 80;
	if (y_max <= 0)
		y_max = 24;
	term_sg = tgetnum("sg");

	x_coord = y_coord = 0;
	termcap_getter("cm", cm_arr);
	termcap_getter("cl", cl_arr);

	if (term_sg <= 0) {
		termcap_getter("so", so_arr);
		termcap_getter("se", se_arr);
	} else
		/* don't mess with stupid standout modes */
		so_arr[0] = se_arr[0] = '\0';
}

void charmode_on() {
	if ((readstream == stdin) && interactive && !tty_charmode) {
		ioctl(0, TCSETA, (char *) (&tty_cbreak));
		tty_charmode++;
	}
}

void charmode_off() {
	if (tty_charmode) {
		ioctl(0, TCSETA, (char *) (&tty_cooked));
		tty_charmode = 0;
	}
}

NODE *lcleartext(NODE *args) {
	printf("%s", cl_arr);
	printf("%s", tgoto(cm_arr, x_margin, y_margin));

	fflush(stdout); /* do it now! */
	fix_turtle_shownness();

	x_coord = x_margin;
	y_coord = y_margin;
	return (UNBOUND);
}

NODE *lcursor(NODE *args) {
	return (cons(make_intnode((FIXNUM) (x_coord - x_margin)), cons(
			make_intnode((FIXNUM) (y_coord - y_margin)), NIL)));
}

NODE *lsetcursor(NODE *args) {
	fix_turtle_shownness();
	NODE *arg;

	arg = pos_int_vector_arg(args);
	if (NOT_THROWING) {
		x_coord = x_margin + getint(car(arg));
		y_coord = y_margin + getint(cadr(arg));
		while ((x_coord >= x_max || y_coord >= y_max) && NOT_THROWING) {
			setcar(args, err_logo(BAD_DATA, arg));
			if (NOT_THROWING) {
				arg = pos_int_vector_arg(args);
				x_coord = x_margin + getint(car(arg));
				y_coord = y_margin + getint(cadr(arg));
			}
		}
	}
	if (NOT_THROWING) {
		printf("%s", tgoto(cm_arr, x_coord, y_coord));
		fflush(stdout);
	}
	return (UNBOUND);
}

NODE *lsetmargins(NODE *args) {
	NODE *arg;

	arg = pos_int_vector_arg(args);
	if (NOT_THROWING) {
		x_margin = getint(car(arg));
		y_margin = getint(cadr(arg));
		lcleartext(NIL);
	}
	return (UNBOUND);
}

NODE *lstandout(NODE *args) {
	char textbuf[300];
	char fmtbuf[100];
	char *old_stringptr = print_stringptr;
	int old_stringlen = print_stringlen;

	sprintf(fmtbuf, "%s%%p%s", so_arr, se_arr);
	print_stringptr = textbuf;
	print_stringlen = 300;
	ndprintf((FILE *) NULL, fmtbuf, car(args));
	*print_stringptr = '\0';
	print_stringptr = old_stringptr;
	print_stringlen = old_stringlen;
	return (make_strnode(textbuf, NULL, (int) strlen(textbuf), STRING, strnzcpy));
}
