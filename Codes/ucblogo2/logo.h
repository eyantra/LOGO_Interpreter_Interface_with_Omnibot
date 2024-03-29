/*
 *      logo.h          logo header file                dvb
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


#ifndef _LOGO_H
#define _LOGO_H

/* #define OBJECTS */

/* #define MEM_DEBUG */

#define ecma	/* for European extended character set using parity bit */



#if !defined(ibm) && !defined(mac)
#ifndef unix
#define unix
#endif
#include "config.h"
#ifndef X_DISPLAY_MISSING
#define x_window
#endif
#endif

#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>


#define check_throwing (stopping_flag == THROWING)

typedef enum {wrapmode, fencemode, windowmode} mode_type;

#define WORDSIZE	(8*sizeof(long))

#define NIL             (NODE *) 0
#define UNBOUND         Unbound
#define UNDEFINED       Unbound
#define END_OF_LIST     (NODE *) 2
#define HASH_LEN        1021	/* a prime number */
#define SEG_SIZE        16000 /* Should be a fairly big number for optimal GC
                                 Performance */
#define MAX_PHYS_LINE   5000
#define MAX_NUMBER      200	/* max number of digits in a float */
#define HIST_MAX	50	/* number of remembered instruction lines */

#define STOP_PRIORITY	0
#define OUTPUT_PRIORITY	1
#define MAYBE_PRIORITY	2
#define TAIL_PRIORITY	2	/* largest tailcall priority */
#define MACRO_PRIORITY	3
#define PREFIX_PRIORITY 4

typedef long int NODETYPES;

#define NT_LOCAL	(NODETYPES)020000000
#define NT_STACK	(NODETYPES)010000000
#define NT_CONT		(NODETYPES)04000000
#define NT_INFIX	(NODETYPES)02000000
#define NT_LINE		(NODETYPES)01000000
#define NT_TAILFORM	(NODETYPES)0400000
#define NT_MACRO	(NODETYPES)0200000
#define NT_TREE		(NODETYPES)0100000
#define NT_EMPTY	(NODETYPES)040000
#define NT_AGGR		(NODETYPES)020000
#define NT_LIST		(NODETYPES)010000
#define NT_RUNP		(NODETYPES)004000
#define NT_ARRAY	(NODETYPES)002000
#define NT_WORD		(NODETYPES)001000
#define NT_NUMBER	(NODETYPES)000400
#define NT_FLOAT	(NODETYPES)000200
#define NT_PRIM		(NODETYPES)000100
#define NT_VBAR		(NODETYPES)000040
#define NT_STRING	(NODETYPES)000020
#define NT_BACKSL	(NODETYPES)000010
#define NT_PUNCT	(NODETYPES)000004
#define NT_COLON	(NODETYPES)000002
#define NT_CASEOBJ	(NODETYPES)000001

#define	PNIL		(NODETYPES)(NT_EMPTY|NT_AGGR|NT_LIST)
#define PUNBOUND	(NODETYPES)0
#define CONS		(NODETYPES)(NT_AGGR|NT_LIST)
#define STRING		(NODETYPES)(NT_WORD|NT_STRING)
#define INT		(NODETYPES)(NT_WORD|NT_NUMBER)
#define FLOATT		(NODETYPES)(NT_WORD|NT_NUMBER|NT_FLOAT)
#define PRIM		(NODETYPES)(NT_PRIM)
#define MACRO		(NODETYPES)(NT_PRIM|NT_MACRO)
#define TAILFORM	(NODETYPES)(NT_PRIM|NT_TAILFORM)
#define CASEOBJ		(NODETYPES)(NT_WORD|NT_CASEOBJ)
#define INFIX		(NODETYPES)(NT_PRIM|NT_INFIX)
#define TREE		(NODETYPES)(NT_AGGR|NT_LIST|NT_TREE)
#define RUN_PARSE	(NODETYPES)(NT_AGGR|NT_LIST|NT_RUNP)
#define QUOTE		(NODETYPES)(NT_WORD|NT_PUNCT)
#define COLON		(NODETYPES)(NT_WORD|NT_PUNCT|NT_COLON)
#define BACKSLASH_STRING (NODETYPES)(NT_WORD|NT_STRING|NT_BACKSL)
#define VBAR_STRING	(NODETYPES)(NT_WORD|NT_STRING|NT_BACKSL|NT_VBAR)
#define ARRAY		(NODETYPES)(NT_AGGR|NT_ARRAY)
#define LINE		(NODETYPES)(NT_LINE|NT_LIST|NT_AGGR)
#define CONT		(NODETYPES)(NT_CONT|NT_LIST)
#define STACK		(NODETYPES)(NT_STACK|NT_LIST)
#define NTFREE		(NODETYPES)(-1)
#define LOCALSAVE	(NODETYPES)(CONS|NT_LOCAL)

#define aggregate(nd)	(nodetype(nd) & NT_AGGR)
#define is_cont(nd)	(nodetype(nd) == CONT)
#define is_list(nd)	(nodetype(nd) & NT_LIST)
#define is_tree(nd)	(nodetype(nd) & NT_TREE)
#define is_string(nd)	(nodetype(nd) & NT_STRING)
#define is_number(nd)	(nodetype(nd) & NT_NUMBER)
#define is_prim(nd)	(nodetype(nd) & NT_PRIM)
#define is_word(nd)	(nodetype(nd) & NT_WORD)
#define runparsed(nd)	(nodetype(nd) & NT_RUNP)
#define backslashed(nd)	(nodetype(nd) & NT_BACKSL)
#define is_tailform(nd)	(nodetype(nd) == TAILFORM)

typedef enum { FATAL, OUT_OF_MEM, STACK_OVERFLOW, TURTLE_OUT_OF_BOUNDS,
		BAD_DATA_UNREC, DIDNT_OUTPUT, NOT_ENOUGH, BAD_DATA, TOO_MUCH,
		DK_WHAT, PAREN_MISMATCH, NO_VALUE, UNEXPECTED_PAREN, DK_HOW,
		NO_CATCH_TAG, ALREADY_DEFINED, STOP_ERROR, ALREADY_DRIBBLING,
		FILE_ERROR, IF_WARNING, SHADOW_WARN, USER_ERR, IS_PRIM,
		NOT_INSIDE, DK_HOW_UNREC, NO_TEST, UNEXPECTED_BRACKET,
		UNEXPECTED_BRACE, BAD_GRAPH_INIT, ERR_MACRO,
		DK_WHAT_UP, AT_TOPLEVEL, APPLY_BAD_DATA, DEEPEND,
		OUT_OF_MEM_UNREC, USER_ERR_MESSAGE, DEEPEND_NONAME,
		BAD_DEFAULT, RUNRES_STOP, MISSING_SPACE,
		CANT_OPEN_ERROR, ALREADY_OPEN_ERROR, NOT_OPEN_ERROR,
		RUNNABLE_ARG,
    /* below this point aren't actually error codes, just messages */
		THANK_YOU, NICE_DAY, NOSHELL_MAC, TYPE_EXIT, ERROR_IN,
		ERRACT_LOOP, PAUS_ING, TRACE_STOPS, TRACE_OUTPUTS,
		NO_FILE, NO_FIONREAD, MEM_LOW, CANT_OPEN, ALREADY_OPEN,
		NOT_OPEN, TRACE_PPROP, WELCOME_TO, CANT_STOP, CANT_GC,
		EXIT_NOW, LOAD_DEF, TRACE_MAKE, EMPTY_PROC, POT_PLIST,
		NO_HELP, NO_HELPON, MORE_HELP,
		MAX_MESSAGE} ERR_TYPES;	    /* MAX_MESSAGE must be last */

typedef int BOOLEAN;

#define FALSE	0
#define TRUE	1

#define SPECIAL_COLORS 5
#define FILLED_COLOR_INDEX 0
#define FILLED_COLOR_OFFSET -5
#define BACKGROUND_COLOR_INDEX 1
#define BACKGROUND_COLOR_OFFSET -4
#define PEN_COLOR_INDEX 2
#define PEN_COLOR_OFFSET -3
#define TEXT_BG_COLOR_INDEX 3
#define TEXT_BG_COLOR_OFFSET -2
#define TEXT_FG_COLOR_INDEX 4
#define TEXT_FG_COLOR_OFFSET -1


#define even_p(x) !(x & 0x1)

#define FIXNUM          long
#define FLONUM          double
#define MAXLOGOINT	0x7fffffff
#define SAFEINT		0x00003fff  /* safe to multiply w/o overflow */

struct string_block {
    unsigned FIXNUM str_refcnt;
    char str_str[1];	    /* This array will be of variable length really */
};

#define getstrrefcnt(sh)        ((sh)->str_refcnt)
#define setstrrefcnt(sh, v)     ((sh)->str_refcnt = (v))
#define incstrrefcnt(sh)        (((sh)->str_refcnt)++)
#define decstrrefcnt(sh)        (--((sh)->str_refcnt))

typedef struct logo_node {
    NODETYPES node_type;
    int my_gen; /* Nodes's Generation */ /*GC*/
    int gen_age; /* How many times to GC at this generation */
    long int mark_gc;	/* when marked */
    struct logo_node *next; /* Link together nodes of the same age */ /*GC*/
    struct logo_node *oldyoung_next;
    union {
	struct {
	    struct logo_node *ncar;
	    struct logo_node *ncdr;
	    struct logo_node *nobj;         /* used only for oblist etc */
	} ncons;
	struct {
	    char *nstring_ptr;
	    struct string_block *nstring_head;
	    FIXNUM nstring_len;
	} nstring;
	struct {
	    struct logo_node * (*nprim_fun) ();
	    short npriority;
	    short nmin_args;
	    short ndef_args;
	    short nmax_args;
	} nprim;
	FIXNUM nint;
	FLONUM nfloat;
	struct {
	    FIXNUM narray_dim;
	    FIXNUM narray_origin;
	    struct logo_node **narray_data;
	} narray;
    } nunion;
} NODE;

#define settype(node, type)     ((node)->node_type = (type))

#define n_car                   nunion.ncons.ncar
#define n_cdr                   nunion.ncons.ncdr
#define n_obj                   nunion.ncons.nobj
#define getobject(node)         ((node)->n_obj)
#define car(node)               ((node)->n_car)
#define cdr(node)               ((node)->n_cdr)
#define caar(node)              ((node)->n_car->n_car)
#define cadr(node)              ((node)->n_cdr->n_car)
#define cdar(node)              ((node)->n_car->n_cdr)
#define cddr(node)              ((node)->n_cdr->n_cdr)

#define n_str                   nunion.nstring.nstring_ptr
#define n_len                   nunion.nstring.nstring_len
#define n_head                  nunion.nstring.nstring_head
#define getstrptr(node)         ((node)->n_str)
#define getstrlen(node)         ((node)->n_len)
#define getstrhead(node)        ((node)->n_head)
#define setstrptr(node,ptr)     ((node)->n_str = (ptr))
#define setstrlen(node,len)     ((node)->n_len = (len))
#define setstrhead(node,ptr)    ((node)->n_head = (ptr))

#define n_int                   nunion.nint
#define getint(node)            ((node)->n_int)
#define setint(node,num)        ((node)->n_int = (num))

#define n_float                 nunion.nfloat
#define getfloat(node)          ((node)->n_float)
#define setfloat(node,num)      ((node)->n_float = (num))

#define n_pfun                  nunion.nprim.nprim_fun
#define n_ppri                  nunion.nprim.npriority
#define n_pmin                  nunion.nprim.nmin_args
#define n_pdef                  nunion.nprim.ndef_args
#define n_pmax                  nunion.nprim.nmax_args
#define getprimfun(node)        ((node)->n_pfun)
#define setprimfun(node,fun)    ((node)->n_pfun = (fun))
#define getprimmin(node)        ((node)->n_pmin)
#define setprimmin(node,num)    ((node)->n_pmin = (num))
#define getprimmax(node)        ((node)->n_pmax)
#define setprimmax(node,num)    ((node)->n_pmax = (num))
#define getprimdflt(node)       ((node)->n_pdef)
#define setprimdflt(node,num)   ((node)->n_pdef = (num))
#define getprimpri(node)        ((node)->n_ppri)
#define setprimpri(node,num)    ((node)->n_ppri = (num))
/* Special value for pmin, means that it's
 *  OK if primitive name on line by itself even though defltargs=1 (ED, CO) */
#define OK_NO_ARG	01000

#define n_dim			nunion.narray.narray_dim
#define n_org			nunion.narray.narray_origin
#define n_array			nunion.narray.narray_data
#define getarrdim(node)		((node)->n_dim)
#define getarrorg(node)		((node)->n_org)
#define getarrptr(node)		((node)->n_array)
#define setarrdim(node,len)	((node)->n_dim = (len))
#define setarrorg(node,org)	((node)->n_org = (org))
#define setarrptr(node,ptr)	((node)->n_array = (ptr))


#define clearparity(ch)		ecma_clear(ch)
#define setparity(ch)		ecma_set(ch)
#define getparity(ch)		ecma_get(ch)
#define ecma_begin		003	/* first char used for quoteds */

typedef enum { RUN, STOP, OUTPUT, THROWING, MACRO_RETURN } CTRLTYPE;

struct segment {
	struct segment *next;
	FIXNUM size;
	struct logo_node nodes[0];
};

#define NOT_THROWING            (stopping_flag != THROWING)
#define RUNNING                 (stopping_flag == RUN)
#define STOPPING                (stopping_flag == STOP)

#define canonical__object(o)    car(o)
#define procnode__object(o)     cadr(o)
#define setprocnode__object(o,v) setcar(cdr(o), v)
#define valnode__object(o)      cadr(cdr(o))
#define setvalnode__object(o,v) setcar(cddr(o), v)
#define plist__object(o)        cadr(cddr(o))
#define setplist__object(o,v)	setcar(cdr(cddr(o)), v)
#define obflags__object(o)	car(cddr(cddr(o)))
#define caselistptr__object(o)  cddr(cddr(o))
#define caselist__object(o)     cdr(cddr(cddr(o)))

#define strnode__caseobj(co)    car(co)
#define object__caseobj(c)      cdr(c)
#define procnode__caseobj(c)    procnode__object(object__caseobj(c))
#define setprocnode__caseobj(c,v) setprocnode__object(object__caseobj(c),v)
#define valnode__caseobj(c)	valnode__object(object__caseobj(c))
#define setvalnode__caseobj(c,v) setvalnode__object(object__caseobj(c),v)
#define plist__caseobj(c)	plist__object(object__caseobj(c))
#define setplist__caseobj(c,v)	setplist__object(object__caseobj(c),v)
#define obflags__caseobj(c)	obflags__object(object__caseobj(c))

#define text__procnode(p)	car(p)
#define formals__procnode(p)    caar(p)
#define bodylist__procnode(p)   cdar(p)
#define bodywords__procnode(p)  cadr(p)
#define setbodywords__procnode(p,v) setcar(cdr(p),v)
#define minargs__procnode(p)    car(cddr(p))
#define dfltargs__procnode(p)   cadr(cddr(p))
#define maxargs__procnode(p)    car(cddr(cddr(p)))

#define unparsed__runparse(rn)  rn
#define parsed__runparse(rn)    getobject(rn)
#define node__quote(q)          car(q)
#define node__colon(c)          car(c)
#define valnode__colon(c)       valnode__caseobj(node__colon(c))

#define unparsed__tree(t)	t
#define treepair__tree(t)	(getobject(t))
#define settreepair__tree(t, v)	setobject(t, v)
#define generation__tree(t)	(car(treepair__tree(t)))
#define setgeneration__tree(t, g) setcar(treepair__tree(t), g)
#define tree__tree(t)		cdr(treepair__tree(t))
#define settree__tree(t, v)	settreepair__tree(t, cons(the_generation, v))

#define unparsed__line(l)	(getobject(l))
#define generation__line(l)	(generation__tree(unparsed__line(l)))
#define tree__line(l)		l

#define cont__cont(c)		(FIXNUM)car(c)
#define val__cont(c)		cdr(c)

/* Object flags.  Ones settable by users via bury_helper must come in threes
 * for proc, val, plist even if meaningless for some of those. */
#define PROC_BURIED	    01
#define VAL_BURIED	    02
#define PLIST_BURIED	    04
#define PROC_TRACED	    010
#define VAL_TRACED	    020
#define PLIST_TRACED	    040
#define PROC_STEPPED	    0100
#define VAL_STEPPED	    0200
#define PLIST_STEPPED	    0400
#define PROC_MACRO	    01000
#define PERMANENT	    02000
#define HAS_GLOBAL_VALUE    04000
#define IS_LOCAL_VALUE	    010000
#define PROC_SPECFORM	    040000

#define setflag__caseobj(c,f) ((obflags__caseobj(c))->n_int |= (f))
#define clearflag__caseobj(c,f) ((obflags__caseobj(c))->n_int &= ~(f))
#define flag__caseobj(c,f) (int)((obflags__caseobj(c))->n_int & (f))
#define flag__object(o,f) (int)((obflags__object(o))->n_int & (f))
#define is_macro(c) (flag__caseobj(c, PROC_MACRO))

#define push(obj, stack)    stack = cons(obj, stack)
#define pop(stack)	    stack = cdr(stack)

/* evaluator labels, needed by macros in other files */
/* (Put the commonly used ones first.) */


#define do_list(x) \
    x(no_reset_args) x(eval_sequence_continue) \
    x(accumulate_arg) x(compound_apply_continue) \
    x(after_const_arg) x(begin_seq) x(begin_apply) \
    x(set_args_continue) x(macro_return) \
    x(repeat_continuation) x(repeat_followup) \
    x(runresult_continuation) x(runresult_followup) \
    x(catch_continuation) x(catch_followup) x(after_lambda) \
    x(after_maybeoutput) \
    x(goto_continuation) \
    x(begin_line) x(end_line) \
    x(all_done) \
    x(fall_off_want_output) x(op_want_stop) x(after_constant)


#define do_enum(x) x,

enum labels {
    do_list(do_enum)
    NUM_TOKENS
};

/* similarly, names that might be translated in Messages file */

#define do_trans(x) \
    x(true) x(false) x(end) x(output) x(stop) x(goto) x(tag) \
    x(if) x(ifelse) x(to) x(macro) x(toplevel) x(system) x(error) x(nothing) \
    x(textscreen) x(splitscreen) x(fullscreen) x(paint) x(erase) x(reverse) \
    x(wrap) x(fence) x(window) x(sum) x(difference) x(product) x(quotient) \
    x(equalp) x(lessp) x(greaterp) x(lessequalp) x(greaterequalp) \
    x(notequalp)

#define wd_enum(x) Name_ ## x,

enum words {
    do_trans(wd_enum)
    NUM_WORDS
};

struct wdtrans {
    NODE *English;
    NODE *Alt;
};

/* evaluator val_status flags, used also in coms.c */

#define VALUE_OK 1	/* [instr instr instr exp] */
#define NO_VALUE_OK 2	/* [instr instr instr instr] */
#define OUTPUT_OK 4	/* [instr instr OUTPUT exp instr] */
#define STOP_OK 8	/* [instr instr STOP instr] */
#define OUTPUT_TAIL 16	/* not [repeat n [... output ...]] */
#define STOP_TAIL 32	/* not [repeat n [... stop ...]] */

/* evaluator registers that need saving around evals */
/* Node pointers must all come before other types.  proc must be first;
   val_status must be first non-node.  See end of init(). */

struct registers {
    NODE *r_proc;   /* the procedure definition */
    NODE *r_argl;   /* evaluated argument list */
    NODE *r_unev;   /* list of unevaluated expressions */
    NODE *r_fun;    /* current function name */
    NODE *r_ufun;   /* current user-defined function name */
    NODE *r_var;    /* frame pointer into var_stack */
    NODE *r_vsp;    /* temp ptr into var_stack */
    NODE *r_qm_list;	/* question mark list */
    NODE *r_formals;	/* list of formal parameters */
    NODE *r_last_ufun;	/* the function that called this one */
    NODE *r_this_line;	/* the current instruction line */
    NODE *r_last_line;	/* the line that called this one */
    NODE *r_current_unode;	/* a pair to identify this proc call */
    NODE *r_didnt_output_name;  /* name of the proc that didn't OP */
    NODE *r_didnt_get_output;	/* procedure wanting output from EVAL */
    FIXNUM r_val_status;    /* tells what EVAL_SEQUENCE should do: */
    FIXNUM r_tailcall;	    /* 0 in sequence, 1 for tail, -1 for arg */
    FIXNUM r_repcount;	    /* count for repeat */
    FIXNUM r_user_repcount;
    FIXNUM r_ift_iff_flag;
};

/* definitions for evaluator registers */

#define proc (regs.r_proc)
#define argl (regs.r_argl)
#define unev (regs.r_unev)
#define fun (regs.r_fun)
#define ufun (regs.r_ufun)
#define var (regs.r_var)
#define vsp (regs.r_vsp)
#define qm_list (regs.r_qm_list)
#define formals (regs.r_formals)
#define last_ufun (regs.r_last_ufun)
#define this_line (regs.r_this_line)
#define last_line (regs.r_last_line)
#define current_unode (regs.r_current_unode)
#define didnt_output_name (regs.r_didnt_output_name)
#define didnt_get_output (regs.r_didnt_get_output)
#define val_status (regs.r_val_status)
#define tailcall (regs.r_tailcall)
#define repcount (regs.r_repcount)
#define user_repcount (regs.r_user_repcount)
#define ift_iff_flag (regs.r_ift_iff_flag)

#define exp expresn


#endif /* _LOGO_H */
