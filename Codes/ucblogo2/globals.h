/*
 *      globals.h       logo global references module           dvb
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

/* main.c */
extern NODE **bottom_stack; /*GC*/
extern NODE *current_line, *exec_list;
extern int main(int, char *[]);
extern void unblock_input(void);
extern NODE **bottom_stack;
extern void delayed_int(void);
extern NODE *command_line;

RETSIGTYPE logo_stop(int);
RETSIGTYPE logo_pause(int);
RETSIGTYPE mouse_down(int);
#define mouse_click mouse_down(0)

#ifndef TIOCSTI
#include <setjmp.h>
extern jmp_buf iblk_buf;
#endif

/* logodata.c */
extern char *strnzcpy(char *, char *, int);
extern char *word_strnzcpy(char *, NODE *, int);
extern char *noparity_strnzcpy(char *, char *, int);
extern char *backslashed_strnzcpy(char *, char *, int);
extern char *mend_strnzcpy(char *, char *, int);
extern char *mend_nosemi(char *, char *, int);
extern char *low_strnzcpy(char *, char *, int);
extern char *cap_strnzcpy(char *, char *, int);
extern char *noparitylow_strnzcpy(char *, char *, int);
extern int low_strncmp(char *, char *, int);
extern int noparity_strncmp(char *, char *, int);
extern int noparitylow_strncmp(char *, char *, int);
extern NODE *make_strnode(char *, struct string_block *, int, NODETYPES,
		char *(*)());
extern void make_runparse(NODE *);
extern NODE *make_quote(NODE *);
extern NODE *maybe_quote(NODE *);
extern NODE *make_caseobj(NODE *, NODE *);
extern NODE *make_colon(NODE *);
extern NODE *make_intnode( FIXNUM);
extern NODE *make_floatnode( FLONUM);
extern NODE *cnv_node_to_numnode(NODE *);
extern NODE *cnv_node_to_strnode(NODE *);
extern NODE *make_static_strnode(char *);
extern NODE *cons_list(int, ...);
extern NODE *make_array( FIXNUM);
extern NODE *llowercase(NODE *);
extern NODE *luppercase(NODE *);
extern NODE *lgprop(NODE *);
extern NODE *lpprop(NODE *);
extern NODE *lremprop(NODE *);
extern NODE *copy_list(NODE *);
extern NODE *lplist(NODE *);
extern int isName(NODE *, enum words);
extern int varTrue(NODE *);
extern NODE *theName(enum words);
extern NODE *TrueName(void);
extern NODE *FalseName(void);

extern char ecma_array[], special_chars[];
extern char ecma_set(int);
extern char ecma_clear(int);
extern int ecma_size;
extern int ecma_get(int);

/* mem.c */
extern NODE *free_list;
extern struct segment *segment_list;
extern NODE *oldyoungs;
extern BOOLEAN inside_gc, int_during_gc;
extern BOOLEAN addseg(void);
extern NODETYPES nodetype(NODE *);
extern void check_valid_oldyoung(NODE *old, NODE *n);
extern void setobject(NODE *, NODE *);
extern void setcar(NODE *, NODE *);
extern void setcdr(NODE *, NODE *);
extern NODE *newnode(NODETYPES);
extern NODE *cons(NODE *, NODE *);
extern void mark(NODE *);
extern void gc(BOOLEAN);
extern NODE *lgc(NODE *);
extern NODE *lnodes(NODE *);
extern NODE *lsetsegsz(NODE *);
extern void fill_reserve_tank(void);
extern void use_reserve_tank(void);
extern void check_reserve_tank(void);

/* parse.c */
extern FILE *loadstream, *writestream, *readstream, *dribblestream;
extern int input_blocking;
extern NODE *current_line, *deepend_proc_name;
extern NODE *reader(FILE *, char *);
extern NODE *parser(NODE *, BOOLEAN);
extern NODE *lparse(NODE *);
extern NODE *runparse(NODE *);
extern NODE *lrunparse(NODE *);

/* math.c */
extern int numberp(NODE *);
extern NODE *lrandom(NODE *);
extern NODE *lrerandom(NODE *);
extern void math_init(void);
extern FLONUM degrad;
extern NODE *ladd(NODE *);
extern NODE *lsub(NODE *);
extern NODE *lmul(NODE *);
extern NODE *ldivide(NODE *);
extern NODE *lremainder(NODE *);
extern NODE *lmodulo(NODE *);
extern NODE *lbitand(NODE *);
extern NODE *lbitor(NODE *);
extern NODE *lbitxor(NODE *);
extern NODE *lashift(NODE *);
extern NODE *llshift(NODE *);
extern NODE *lbitnot(NODE *);
extern NODE *lsin(NODE *);
extern NODE *lcos(NODE *);
extern NODE *latan(NODE *);
extern NODE *lradsin(NODE *);
extern NODE *lradcos(NODE *);
extern NODE *lradatan(NODE *);
extern NODE *lsqrt(NODE *);
extern NODE *linteg(NODE *);
extern NODE *lroundx(NODE *);
extern NODE *lexp(NODE *);
extern NODE *llog10(NODE *);
extern NODE *lln(NODE *);
extern NODE *lpower(NODE *);
extern NODE *torf(BOOLEAN);
extern NODE *llessp(NODE *);
extern NODE *lgreaterp(NODE *);
extern NODE *llessequalp(NODE *);
extern NODE *lgreaterequalp(NODE *);
extern int compare_node(NODE *, NODE *, BOOLEAN);
extern BOOLEAN equalp_help(NODE *, NODE *, BOOLEAN);
extern NODE *lequalp(NODE *);
extern NODE *lnotequalp(NODE *);
extern NODE *l_eq(NODE *);
extern NODE *lbeforep(NODE *);

/* intern.c */
extern NODE *hash_table[HASH_LEN];
void map_oblist(void(*)());
extern NODE *make_instance(NODE *, NODE *);
extern NODE *intern(NODE *);

/* print.c */
extern int print_stringlen;
extern char *print_stringptr;
extern int force_printwidth, force_printdepth;
extern int x_margin, y_margin;
extern NODE *Fullprintp;
extern void update_coords(char);
extern void print_char(FILE *, char);
extern void print_space(FILE *);
extern void ndprintf(FILE *, char *, ...);
extern void real_print_help(FILE *, NODE *, int, int);
extern void print_help(FILE *, NODE *);
extern void print_node(FILE *, NODE *);
extern void print_nobrak(FILE *, NODE *);
extern void new_line(FILE *);
extern NODE *lshow(NODE *);
extern NODE *ltype(NODE *);
extern NODE *lprint(NODE *);

/* init.c */
extern NODE *Left_Paren, *Right_Paren;
extern NODE *Redefp, *Caseignoredp, *Erract, *Printdepthlimit;
extern NODE *Printwidthlimit, *Pause, *LoadNoisily, *AllowGetSet;
extern NODE *UnburyOnEdit, *Make, *Listvalue, *Dotsvalue;
extern NODE *Unbound, *Not_Enough_Node, *Buttonact, *LogoVersion;
extern NODE *Minus_Sign, *Minus_Tight, *Startup, *Startuplg, *Query;
extern NODE *UseAlternateNames, *LogoLogo, *LogoPlatform, *Keyact;
extern NODE *CommandLine, *Null_Word;
extern void init(void);
extern struct wdtrans translations[];
extern char *LogoPlatformName;

/* wrksp.c */

extern char *editor, *editorname, *tempdir;
extern int to_pending;
extern NODE *ltext(NODE *);
extern NODE *lfulltext(NODE *);
extern NODE *ldefine(NODE *);
extern NODE *ldefmacro(NODE *);
extern NODE *anonymous_function(NODE *);
extern NODE *lmacro(NODE *);
extern NODE *lto(NODE *);
extern NODE *lmake(NODE *);
extern NODE *llocal(NODE *);
extern NODE *lglobal(NODE *);
extern NODE *cnt_list, *cnt_last;
extern NODE *lcontents(NODE *);
extern NODE *lburied(NODE *);
extern NODE *ltraced(NODE *);
extern NODE *lstepped(NODE *);
extern NODE *lprocedures(NODE *);
extern NODE *lprimitives(NODE *);
extern NODE *lnames(NODE *);
extern NODE *lplists(NODE *);
extern NODE *lpo(NODE *);
extern NODE *lpot(NODE *);
extern NODE *lerase(NODE *);
extern NODE *lerall(NODE *);
extern NODE *lerps(NODE *);
extern NODE *lerns(NODE *);
extern NODE *lerpls(NODE *);
extern NODE *lbury(NODE *);
extern NODE *ltrace(NODE *);
extern NODE *lstep(NODE *);
extern NODE *lburiedp(NODE *);
extern NODE *ltracedp(NODE *);
extern NODE *lsteppedp(NODE *);
extern NODE *lunbury(NODE *);
extern NODE *luntrace(NODE *);
extern NODE *lunstep(NODE *);
extern char *addsep(char *);
extern NODE *ledit(NODE *);
extern NODE *leditfile(NODE *);
extern NODE *lthing(NODE *);
extern NODE *lnamep(NODE *);
extern NODE *lprocedurep(NODE *);
extern NODE *lplistp(NODE *);
extern NODE *lprimitivep(NODE *);
extern NODE *ldefinedp(NODE *);
extern NODE *lmacrop(NODE *);
extern NODE *lcopydef(NODE *);
extern NODE *lhelp(NODE *);
extern NODE *larity(NODE *);

/* error.c */
extern char *message_texts[];
extern NODE *throw_node;
extern NODE *err_mesg;
extern ERR_TYPES erract_errtype;
extern void err_print(char *);
extern NODE *err_logo(ERR_TYPES, NODE *);
extern NODE *lerror(NODE *);
extern NODE *lpause(NODE *);
extern NODE *lcontinue(NODE *);

/* eval.c */
extern NODE *var_stack;
extern NODE *output_node, *output_unode, *last_call;
extern CTRLTYPE stopping_flag;
extern char *logolib, *helpfiles, *csls;
extern FIXNUM dont_fix_ift;
extern void eval_driver(NODE *);
extern NODE *err_eval_driver(NODE *, BOOLEAN);
extern NODE *lapply(NODE *);
extern NODE *lqm(NODE *);
extern NODE *deep_copy(NODE *);
extern void tell_shadow(NODE *);
extern int not_local(NODE *, NODE *);
extern int num_saved_nodes;
extern struct registers regs;
extern NODE *Regs_Node;
extern int inside_evaluator;
extern NODE *eval_buttonact;

/*
 #ifdef OBJECTS
 extern NODE *val_eval_driver(NODE *seq);
 #endif
 */

/* lists.c */
extern NODE *lbutfirst(NODE *);
extern NODE *lbutlast(NODE *);
extern NODE *lfirst(NODE *);
extern NODE *lfirsts(NODE *);
extern NODE *lbfs(NODE *);
extern NODE *llast(NODE *);
extern NODE *llist(NODE *);
extern NODE *lemptyp(NODE *);
extern NODE *lascii(NODE *);
extern NODE *lrawascii(NODE *);
extern NODE *lvbarredp(NODE *);
extern NODE *lchar(NODE *);
extern NODE *lcount(NODE *);
extern NODE *lfput(NODE *);
extern NODE *llput(NODE *);
extern NODE *string_arg(NODE *);
extern NODE *lword(NODE *);
extern NODE *lsentence(NODE *);
extern NODE *lwordp(NODE *);
extern NODE *llistp(NODE *);
extern NODE *lnumberp(NODE *);
extern NODE *larrayp(NODE *);
extern NODE *lmemberp(NODE *);
extern NODE *lsubstringp(NODE *);
extern NODE *lmember(NODE *);
extern NODE *integer_arg(NODE *);
extern FIXNUM int_arg(NODE *);
extern NODE *litem(NODE *);
extern NODE *lsetitem(NODE *);
extern NODE *l_setitem(NODE *);
extern NODE *larray(NODE *);
extern NODE *larraytolist(NODE *);
extern NODE *llisttoarray(NODE *);
extern NODE *lform(NODE *);
extern NODE *l_setfirst(NODE *);
extern NODE *l_setbf(NODE *);

/* files.c */

extern NODE *file_list;
extern NODE *reader_name, *writer_name, *file_prefix;
extern NODE *lseteditor(NODE *);
extern NODE *lsetlibloc(NODE *);
extern NODE *lsetcslsloc(NODE *);
extern NODE *lsethelploc(NODE *);
extern NODE *lsettemploc(NODE *);
extern NODE *ldribble(NODE *);
extern NODE *lnodribble(NODE *);
extern NODE *lopenread(NODE *);
extern NODE *lopenwrite(NODE *);
extern NODE *lopenappend(NODE *);
extern NODE *lopenupdate(NODE *);
extern NODE *lallopen(NODE *);
extern NODE *lclose(NODE *);
extern NODE *lsetwrite(NODE *);
extern NODE *lsetread(NODE *);
extern NODE *lreader(NODE *);
extern NODE *lwriter(NODE *);
extern NODE *lerasefile(NODE *);
extern NODE *lsave(NODE *);
extern void silent_load(NODE *, char *);
extern NODE *lload(NODE *);
extern NODE *lcslsload(NODE *);
extern NODE *lsetprefix(NODE *);
extern NODE *lprefix(NODE *);
extern NODE *lreadlist(NODE *);
extern NODE *lreadword(NODE *);
extern NODE *lreadrawline(NODE *);
extern NODE *lreadchar(NODE *);
extern NODE *lreadchars(NODE *);
extern NODE *leofp(NODE *);
extern NODE *lkeyp(NODE *);
extern NODE *lreadpos(NODE *);
extern NODE *lsetreadpos(NODE *);
extern NODE *lwritepos(NODE *);
extern NODE *lsetwritepos(NODE *);
extern int readchar_lookahead_buf;
extern int need_save;
extern NODE *save_name;

/* coms.c */
extern NODE *make_cont(enum labels, NODE *);
extern NODE *loutput(NODE *);
extern NODE *lstop(NODE *);
extern NODE *lthrow(NODE *);
extern NODE *lcatch(NODE *);
extern NODE *lgoto(NODE *);
extern NODE *ltag(NODE *);
extern NODE *lnot(NODE *);
extern NODE *land(NODE *);
extern NODE *lor(NODE *);
extern NODE *lif(NODE *);
extern NODE *lifelse(NODE *);
extern NODE *lrun(NODE *);
extern NODE *lrunresult(NODE *);
extern NODE *pos_int_arg(NODE *);
extern int torf_arg(NODE *);
extern NODE *lrepeat(NODE *);
extern NODE *lrepcount(NODE *);
extern NODE *lforever(NODE *);
extern NODE *ltest(NODE *);
extern NODE *liftrue(NODE *);
extern NODE *liffalse(NODE *);
extern void prepare_to_exit(BOOLEAN);
extern NODE *lbye(NODE *);
extern NODE *lwait(NODE *);
extern NODE *lshell(NODE *);
NODE *runnable_arg(NODE *);

/* term.c */
#ifndef HAVE_WX
extern int x_coord, y_coord, x_max, y_max;
#endif
extern int interactive;
extern void term_init(void);
extern void charmode_on(void);
extern void charmode_off(void);
extern NODE *lcleartext(NODE *);
extern NODE *lcursor(NODE *);
extern NODE *lsetcursor(NODE *);
extern NODE *lsetmargins(NODE *);
extern NODE *lstandout(NODE *);

/* libloc.c */
extern char *libloc, *helploc, *cslsloc, *temploc, *separator;

/* paren.c */
extern NODE *the_generation;
extern void untreeify_proc(NODE *);
extern void make_tree_from_body(NODE *);
extern void make_tree(NODE *);
extern NODE *tree_dk_how;
extern void check_library(NODE *);

/* graphics.c */

extern mode_type current_mode;
extern FLONUM turtle_x, turtle_y, turtle_heading, x_scale, y_scale;
extern BOOLEAN turtle_shown;
extern BOOLEAN refresh_p;
extern FIXNUM g_round( FLONUM);
void draw_turtle(void);
extern NODE *numeric_arg(NODE *);
extern NODE *lright(NODE *);
extern NODE *lleft(NODE *);
extern NODE *lforward(NODE *);
extern NODE *lback(NODE *);
extern NODE *lshowturtle(NODE *);
extern NODE *lhideturtle(NODE *);
extern NODE *lshownp(NODE *);
extern NODE *lsetheading(NODE *);
extern NODE *lheading(NODE *);
extern NODE *pos_int_vector_arg(NODE *);
extern NODE *ltowards(NODE *);
extern NODE *lpos(NODE *);
extern NODE *lscrunch(NODE *);
extern NODE *lhome(NODE *);
extern NODE *lclearscreen(NODE *);
extern NODE *lclean(NODE *);
extern NODE *lsetpos(NODE *);
extern NODE *lsetxy(NODE *);
extern NODE *lsetx(NODE *);
extern NODE *lsety(NODE *);
extern NODE *lwrap(NODE *);
extern NODE *lfence(NODE *);
extern NODE *lwindow(NODE *);
extern NODE *lfill(NODE *);

extern NODE *llabel(NODE *);
extern NODE *ltextscreen(NODE *);
extern NODE *lsplitscreen(NODE *);
extern NODE *lfullscreen(NODE *);
extern NODE *lpendownp(NODE *);
extern NODE *lpenmode(NODE *);
extern NODE *lpencolor(NODE *);
extern NODE *lbackground(NODE *);
extern NODE *lpensize(NODE *);
extern NODE *lpenpattern(NODE *);
extern NODE *lpendown(NODE *);
extern NODE *lpenup(NODE *);
extern NODE *lpenpaint(NODE *);
extern NODE *lpenerase(NODE *);
extern NODE *lpenreverse(NODE *);
extern NODE *lsetpencolor(NODE *);
extern NODE *lsetbackground(NODE *);
extern NODE *lsetpalette(NODE *);
extern NODE *lpalette(NODE *);
extern NODE *lsetpensize(NODE *);
extern NODE *lsetpenpattern(NODE *);
extern NODE *lsetscrunch(NODE *);
extern NODE *lmousepos(NODE *);
extern NODE *lclickpos(NODE *);
extern NODE *lbuttonp(NODE *);
extern NODE *lbutton(NODE *);
extern NODE *ltone(NODE *);
extern NODE *larc(NODE *);
extern NODE *lrefresh(NODE *);
extern NODE *lnorefresh(NODE *);
extern NODE *lloadpict(NODE *);
extern NODE *lsavepict(NODE *);
extern NODE *lepspict(NODE *);
extern void redraw_graphics(void);
extern NODE *lscreenmode(NODE *);
extern NODE *lturtlemode(NODE *);
extern void fix_turtle_shownness(void);
extern enum s_md {
	SCREEN_TEXT, SCREEN_SPLIT, SCREEN_FULL
} screen_mode;

extern int in_graphics_mode, in_splitscreen, cur_len, read_index;
#define rd_putc putc

/* mymethods.c */


