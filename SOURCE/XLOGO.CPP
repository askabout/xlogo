#include <graphics.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <dos.h>
#include <string.h>
#include <bios.h>
#include <alloc.h>
#include <dir.h>

#include "xlogo.h"
#include "graph.h"
#include "console.h"
#include "status.h"
#include "keyboard.h"
#include "commands.h"
#include "menu.h"
#include "lexem.h"
#include "inter.h"
#include "synt.h"
#include "bufer.h"
#include "edit.h"
#include "saveload.h"
#include "help.h"
#include "listbox.h"
#include "dialogs.h"

static char	program_name[80];
static int	mode_ed;
static int	pen_state;
static int	cur_string, cur_pos	= 0;

static int	program_errpos;
static int	program_error;
static int	program_errflg		= 0;
static int	program_saved		= 1;
static int	program_quit		= 0;

static int	curx, cury, curcolor, curangle, penstate, rnd;

static void	editfunc (int *, int *, char *);

void get_sys_state (char *nam, int *sav, int *mod, int *str, int *pos, int *ps)
{
	strcpy (nam, program_name);
	*mod	= mode_ed;
	*str	= cur_string;
	*pos	= cur_pos + 1;
	*sav	= program_saved;
	*ps	= pen_state;
}

void menu_init ()
{
	int i;

	i = menu_add ("Файл");
	menu_additem (i, "Создать",			CMDSYS_NEW);
	menu_additem (i, "Загрузить...        F3",	CMDSYS_LOAD);
	menu_additem (i, "Сохранить           F2",	CMDSYS_SAVE);
	menu_additem (i, "Сохранить как... ",		CMDSYS_SAVEAS);
	menu_additem (i, "", 0);
	menu_additem (i, "Выход            Alt-X",	CMDSYS_QUIT);
	i = menu_add ("Правка");
	menu_additem (i, "Редактирование F5",		CMDSYS_EDIT);
	menu_additem (i, "Замена         F6",		CMDSYS_CHANGE);
	menu_additem (i, "Вставка        F7",		CMDSYS_INSERT);
	i = menu_add ("Программа");
	menu_additem (i, "Листинг          F8",		CMDSYS_LIST);
	menu_additem (i, "Запуск           F9",		CMDSYS_RUN);
	menu_additem (i, "Статус пера      F4", 	CMDSYS_PENSHOW);
	menu_additem (i, "", 0);
	menu_additem (i, "Количество строк",		CMDSYS_LINES);
	i = menu_add ("Помощь");
	menu_additem (i, "Содержание            F1",	CMDSYS_CONTENTS);
	menu_additem (i, "Индекс          Shift+F1",	CMDSYS_INDEX);
	menu_additem (i, "Поиск темы       Ctrl+F1",	CMDSYS_TOPSEARCH);
	menu_additem (i, "Предыдущая тема   Alt+F1",	CMDSYS_PREVTOPIC);
	menu_additem (i, "", 0);
	menu_additem (i, "О программе...",		CMDSYS_ABOUT);
	menubar_draw ();
}

void error_con (int err, int numstr, int posstr)
{
	char errstr [200] = "", errmsg [200] = "", c;

	strcpy (errstr, errors (err));
	if (numstr >= 0)
		sprintf (errmsg, ", %d:%d", numstr, posstr + 1);
	strcat (errstr, errmsg);
	switch (err / 100) {
		case 0 : c = 12;	break;	// Интерпретатор
		case 1 : c = 2; 	break;	// Без комментариев
		case 2 : c = 13;	break;	// Синтаксический анализатор
		case 3 : c = 7;		break;	// Файлы
		case 4 : c = 9;		break;	// Помощь
		case 5 : c = 3;		break;	// Лексический анализатор
		case 7 : c = 4;		break;	// Буфер
	}
	console_message (errstr, c);
}

int break_dialog ()
{
	int	err = 0;

	dialog_init (220, 200, 420, 280, "Вопрос");
	dialog_statictext (10, 10, WHITE, "Программа была прервана");
	dialog_statictext (52, 30, WHITE, "Продолжить ?");
	dialog_button (DLG_YES, 28, 60, 8, "Да");
	dialog_button (DLG_NO, 108, 60, 8, "Нет");
	switch (dialog_run ()) {
		case DLG_NO:
			err = INT_ERR_USER_BREAK;
			break;
	}
	dialog_done ();
	return err;
}

int about_dialog ()
{
	dialog_init (220, 130, 420, 290, "О программе...");
	putimage (243, 155, logotype, COPY_PUT);
	dialog_statictext (32, 90, BLACK, "ВКИ НГУ 1999-2000");
	dialog_button (DLG_OK, 68, 125, 8, "OK");
	dialog_run ();
	dialog_done ();
	return 0;
}

char *get_cmdstr (char *s, Lexem *lex)
{
	Lexem	*o	= lex;
	char	t[8], *p, *x;

	while (o -> type != LEX_ENDSTR) o++;
	memset (s, ' ', o -> pos_str);
	s [o -> pos_str] = 0;
	while (lex -> type != LEX_ENDSTR) {
		p = s + lex -> pos_str;
		switch (lex -> type) {
			case LEX_CMD:
				if ((lex + 1) -> pos_str < lex -> pos_str)
					*p = '=';
				else {
					x = getname (lex -> value);
					strncpy (p, x, strlen (x));
				}
				break;
			case LEX_IDENT:
				x = get_id (lex -> value);
				strncpy (p, x, strlen (x));
				break;
			case LEX_NUM:
				x = itoa (lex -> value, t, 10);
				strncpy (p, x, strlen (x));
				break;
			case LEX_PARMSEP:
				*p = ',';
				break;
			case LEX_OPERAD:
				*p     = lex -> value;
				if (lex -> value > 0xFF)
					*(p+1) = lex -> value >> 8;
				break;
			case LEX_OTKRSK:
				*p = '(';
				break;
			case LEX_ZAKRSK:
				*p = ')';
				break;
		}
		lex++;
	}
	return s;
}

int func_values ()
{
	int x, y, c, p, a;

	get_inter_stat (&c, &p, &x, &y, &a);
	put_nz_id (curx,	x);
	put_nz_id (cury,	y);
	put_nz_id (curcolor,	c);
	put_nz_id (curangle,	a);
	put_nz_id (penstate,	p);
	put_nz_id (rnd,		rand ());
	return 0;
}

int ident_init ()
{
	int i;

	put_id ("BLACK",		&i);
	put_nz_id (i, BLACK);
	put_id ("BLUE",			&i);
	put_nz_id (i, BLUE);
	put_id ("GREEN",		&i);
	put_nz_id (i, GREEN);
	put_id ("CYAN",			&i);
	put_nz_id (i, CYAN);
	put_id ("RED",			&i);
	put_nz_id (i, RED);
	put_id ("MAGENTA",		&i);
	put_nz_id (i, MAGENTA);
	put_id ("BROWN",		&i);
	put_nz_id (i, BROWN);
	put_id ("LIGHTGRAY",		&i);
	put_nz_id (i, LIGHTGRAY);
	put_id ("DARKGRAY",		&i);
	put_nz_id (i, DARKGRAY);
	put_id ("LIGHTBLUE",		&i);
	put_nz_id (i, LIGHTBLUE);
	put_id ("LIGHTGREEN",		&i);
	put_nz_id (i, LIGHTGREEN);
	put_id ("LIGHTCYAN",		&i);
	put_nz_id (i, LIGHTCYAN);
	put_id ("LIGHTRED",		&i);
	put_nz_id (i, LIGHTRED);
	put_id ("LIGHTMAGENTA",		&i);
	put_nz_id (i, LIGHTMAGENTA);
	put_id ("YELLOW",		&i);
	put_nz_id (i, YELLOW);
	put_id ("WHITE",		&i);
	put_nz_id (i, WHITE);
	put_id ("MAXX",			&i);
	put_nz_id (i, get_max_x ());
	put_id ("MAXY",			&i);
	put_nz_id (i, get_max_y ());
	put_id ("CURX",			&curx);
	put_id ("CURY",			&cury);
	put_id ("CURCOLOR",		&curcolor);
	put_id ("CURANGLE",		&curangle);
	put_id ("PENSTATE",		&penstate);
	put_id ("RANDOM",		&rnd);
	func_values ();
	return 0;
}

int tracking (int newstr)
{
	int numstr;

	if (newstr < cur_string && newstr <= Buf_Sum_Str () || program_errflg) {
		inter_reset ();
		ident_reset ();
		program_error = ident_init ();
		if (program_error)
			return program_error;
		numstr = 1;
		if (get_bp_flag (numstr)) {
			error_con (ERR_BREAKPOINT, numstr, 0);
			program_errflg	= 0;
			newstr		= numstr;
		}
	} else {
		numstr = cur_string;
		if (pen_state)
			draw_pen ();
	}
	program_error = 0;
	while (numstr < newstr) {
		if (!program_error)
			program_error	= inter (&numstr, &program_errpos);
		program_errflg	= 0;
		func_values ();
		if (program_error == INT_ERR_USER_BREAK) {
			program_error	= break_dialog ();
			cur_string	= numstr;
		}
		if (program_error) {
			error_con (program_error, numstr, program_errpos);
			program_errflg	= numstr != cur_string;
			mode_ed		= MODE_EDIT;
			break;
		}
		numstr++;
		if (get_bp_flag (numstr)) {
			error_con (ERR_BREAKPOINT, numstr, 0);
			break;
		}
	}
	if (pen_state)
		draw_pen ();
	cur_string = numstr;
	if (cur_string > Buf_Sum_Str ())
		mode_ed = MODE_INSERT;
	return 0;
}

int nextpar (Lexem* &lex, int *posstr, int *value)
{
	if (lex -> type == LEX_ENDSTR)
		return (1);
	lex++;
	if (lex -> type == LEX_ENDSTR)
		return (1);
	*value	= lex -> value;
	*posstr	= lex -> pos_str;
	lex++;
	return 0;
}

char *list (char *s, int i, int cur)
{
	sprintf (s, "%03d%c", i, i == cur ? '>' : ' ');
	get_cmdstr (s + strlen (s), get_cmd (i));
	return (s);
}

int dialog_curstring (int mode, int *n)
{
	int i, nstr = Buf_Sum_Str () + 1;
	char s[256], *x;
	EditSettings es;

	if (nstr < 2)
		return ERR_CANCEL;
	*n = cur_string;
	save_settings ();
	switch (mode) {
		case MODE_INSERT:
			x = "Выбор позиции вставки";
			break;
		case MODE_CHANGE:
			x = "Выбор строки замены";
			break;
		case MODE_EDIT:
			x = "Выбор строки редактирования";
			break;
	}
	openhwindow (192, 150, 448, 330, x);
	listbox_init (200, 160, 30, 20, NULL, NULL);
	for (i = 1; i < nstr; i++) {
		list (s, i, cur_string);
		listbox_add (s, strlen (s) + 1, get_bp_flag (i) ? LIGHTRED : WHITE);
	}
	x = "<новая строка>";
	listbox_add (x, strlen (x) + 1, WHITE);
	(*n)--;
	i = listbox_run (n, s);
	(*n)++;
	listbox_done ();
	closewindow ();
	load_settings ();
	if (!i)
		mode_ed = *n == nstr ? MODE_INSERT : mode;
	else
		return ERR_CANCEL;
	return 0;
}

int listing (int from, int to)
{
	char	s[200];
	int	key, i, j;

	console_message ("", CON_TXT_COLOR);
	for (i = from; i <= to; i++) {
		console_message (list (s, i, cur_string), get_bp_flag (i) ? LIGHTRED : CON_TXT_COLOR);
		if (!(i % (CON_HGT-2)) && i != to) {
			console_message ("Дальше ?", CON_MSG_COLOR);
			key = bioskey (_KEYBRD_READ);
			if (key == K_ESCAPE)
				break;
		}
	}
	return 0;
}

int cmdsys_run ()
{
	if (cur_string > Buf_Sum_Str ())
		program_errflg = 1;
	return tracking (Buf_Sum_Str () + 1);
}

int warning_overwrite ()
{
	int	err = 0;

	dialog_init (160, 200, 480, 280, "Вопрос");
	dialog_statictext (10, 10, WHITE, "Файл с указанным именем уже существует");
	dialog_statictext (104, 30, WHITE, "Перезаписать ?");
	dialog_button (DLG_YES, 48, 60, 8, "Да");
	dialog_button (DLG_CANCEL, 128, 60, 8, "Нет");
	dialog_button (DLG_CANCEL, 208, 60, 8, "Отмена");
	switch (dialog_run ()) {
		case DLG_CANCEL:
			err = ERR_CANCEL;
			break;
	}
	dialog_done ();
	return err;
}

int warning_quit ()
{
	int	err = 0;

	dialog_init (168, 210, 472, 270, "Вопрос");
	dialog_statictext (10, 10, WHITE, "Вы точно хотите выйти из программы ?");
	dialog_button (DLG_YES, 40, 40, 8, "Да");
	dialog_button (DLG_CANCEL, 120, 40, 8, "Нет");
	dialog_button (DLG_CANCEL, 200, 40, 8, "Отмена");
	switch (dialog_run ()) {
		case DLG_CANCEL:
			err = ERR_CANCEL;
			break;
	}
	dialog_done ();
	return err;
}

int cmdsys_saveas ()
{
	FILE	*f;
	char	s[MAXPATH];
	int	err;

	err	= select_file (s, "Сохранить файл");
	f	= fopen (s, "r+");
	if (f) {
		fclose (f);
		err = warning_overwrite ();
	}
	if (!err)
		err = id_optimize ();
	if (!err && !(err = save_file (s))) {
		strcpy (program_name, s);
		sprintf (s, "Программа была сохранена как '%s'", program_name);
		console_message (s, CON_MSG_COLOR);
		program_saved = 1;
	}
	if (!err)
		err = ident_init ();
	return err;
}

int cmdsys_save ()
{
	char	s[MAXPATH];
	int	err;

	strcpy (s, program_name);
	if (!strcmp (program_name, NAME_DEF))
		err = cmdsys_saveas ();
	else {
		if (!err)
			err = id_optimize ();
		if (!(err = save_file (s))) {
			strcpy (program_name, s);
			program_saved = 1;
			console_message ("Программа была сохранена", CON_MSG_COLOR);
		}
		if (!err)
			err = ident_init ();
	}
	return err;
}

int warning_save ()
{
	int	err = 0;

	if (program_saved)
		return (0);
	dialog_init (200, 200, 440, 280, "Вопрос");
	dialog_statictext (10, 10, WHITE, "Программа не была сохранена");
	dialog_statictext (74, 30, WHITE, "Сохранить ?");
	dialog_button (DLG_YES, 8, 60, 8, "Да");
	dialog_button (DLG_NO, 88, 60, 8, "Нет");
	dialog_button (DLG_CANCEL, 168, 60, 8, "Отмена");
	switch (dialog_run ()) {
		case DLG_YES:
			err = cmdsys_save ();
			break;
		case DLG_CANCEL:
			err = ERR_CANCEL;
			break;
	}
	dialog_done ();
	return err;
}

void print_header ()
{
	char	s[CON_VSYM + 1], m[CON_MSYM];

	memset (s, '─', CON_VSYM);
	s [CON_VSYM] = 0;
	sprintf (m, " Консоль XLogo v%s", VERSION);
	console_message ("", CON_MSG_COLOR);
	console_message (s, CON_MSG_COLOR);
	console_message (m, CON_MSG_COLOR);
	console_message (s, CON_MSG_COLOR);
	console_message ("", CON_MSG_COLOR);
}

int program_load (char *s)
{
	int err;

	err = load_file (s);
	if (err)
		return err;
	strcpy (program_name, strupr (s));
	cur_string	= 1;
	program_saved	= 1;
	pen_state	= 1;
	mode_ed		= MODE_INSERT;
	inter_reset ();
	ident_init ();
	draw_pen ();
	syntax_init ();
	print_header ();
	sprintf (s, "Загружена программа '%s'", program_name);
	console_message (s, CON_MSG_COLOR);
	err = cmdsys_run ();
	status_refresh ();
	return err;
}

int cmdsys_load ()
{
	char	s[MAXPATH];
	int	err;

	if (!(err = warning_save ()))
		if (!(err = select_file (s, "Загрузить файл")))
			err = program_load (s);
	return err;
}

int cmdsys_new ()
{
	int	err;

	if (!(err = warning_save ())) {
		strcpy (program_name, NAME_DEF);
		mode_ed		= MODE_INSERT;
		cur_string	= 1;
		program_saved	= 1;
		pen_state	= 1;
		inter_reset ();
		draw_pen ();
		syntax_init ();
		buf_init ();
		lex_init ();
		ident_init ();
		print_header ();
		status_refresh ();
	}
	return err;
}

int cmdsys_quit ()
{
	int err;

	program_quit = !(err = warning_quit ()) && !(err = warning_save ());
	return err;
}

int cmdsys_del (Lexem *lex, int *posstr)
{
	int par1, par2, err = 0;

	nextpar (lex, posstr, &par1);
	if (par1 < 1 || par1 > Buf_Sum_Str ())
		return ERR_INVSTR;
	if (nextpar (lex, posstr, &par2))
		par2 = par1;
	else if (par2 < par1 || par2 > Buf_Sum_Str ())
		return ERR_INVSTR;
	do {
		err = del_cmd (par1);
	} while (par1 < par2-- && !err);
	if (!err) {
		program_errflg	= 1;
		err		= cmdsys_run ();
		posstr		= 0;
	}
	return err;
}

int cmdsys_bptoggle (Lexem *lex, int *posstr)
{
	int par, err = 0;
	char s [80];

	nextpar (lex, posstr, &par);
	if (!(err = toggle_bp_flag (par))) {
		sprintf (s, "%s %d", get_bp_flag (par) ? "Установлена точка останова на строке" : "Убрана точка останова со строки", par);
		console_message (s, CON_MSG_COLOR);
	}
	return err;
}

int cmdsys_bpclear ()
{
	int err = bp_reset ();
	if (!err)
		console_message ("Убраны все точки останова", CON_MSG_COLOR);
	return err;
}

int cmdsys_list (Lexem *lex, int *posstr)
{
	int par1, par2;

	if (nextpar (lex, posstr, &par1))
		par1 = 1;
	else if (par1 < 1 || par1 > Buf_Sum_Str ())
		return ERR_INVSTR;
	if (nextpar (lex, posstr, &par2))
		par2 = Buf_Sum_Str ();
	else if (par2 < par1 || par2 > Buf_Sum_Str ())
		return ERR_INVSTR;
	return listing (par1, par2);
}

int cmdsys_chmod (Lexem *lex, int *posstr, int mode)
{
	int par, n;

	if (nextpar (lex, posstr, &par))
		par = cur_string;
	else if (par < 1 || par > Buf_Sum_Str () + 1)
		return ERR_INVSTR;
	mode_ed = mode;
	return tracking (par);
}

int cmdsys_lines ()
{
	char s[8];

	console_message (itoa (Buf_Sum_Str (), s, 10), CON_TXT_COLOR);
	return 0;
}

int cmdsys_penstate (int state)
{
	if (pen_state != state)
		draw_pen ();
	pen_state = state;
	return 0;
}

int cmdsys_print (Lexem *lex, int *posstr)
{
	int	err, res, pos = 1;
	char	s[8];

	err = exec_ver (&res, lex, &pos, posstr);
	if (err)
		return (err);
	console_message (itoa (res, s, 10), CON_MSG_COLOR);
	return 0;
}

int inter_syscmd (Lexem *lex, int *posstr)
{
	int err = 0;

	func_values ();
	switch (lex -> value) {
		case CMDSYS_LOAD:
			err = cmdsys_load ();
			break;
		case CMDSYS_SAVE:
			err = cmdsys_save ();
			break;
		case CMDSYS_NEW:
			err = cmdsys_new ();
			break;
		case CMDSYS_DEL:
			err = cmdsys_del (lex, posstr);
			break;
		case CMDSYS_LIST:
			err = cmdsys_list (lex, posstr);
			break;
		case CMDSYS_QUIT:
			err = cmdsys_quit ();
			break;
		case CMDSYS_INSERT:
			err = cmdsys_chmod (lex, posstr, MODE_INSERT);
			break;
		case CMDSYS_CHANGE:
			err = cmdsys_chmod (lex, posstr, MODE_CHANGE);
			break;
		case CMDSYS_EDIT:
			err = cmdsys_chmod (lex, posstr, MODE_EDIT);
			break;
		case CMDSYS_RUN:
			err = cmdsys_run ();
			break;
		case CMDSYS_LINES:
			err = cmdsys_lines ();
			break;
		case CMDSYS_PRINT:
			err = cmdsys_print (lex, posstr);
			break;
		case CMDSYS_PENHIDE:
			err = cmdsys_penstate (0);
			break;
		case CMDSYS_PENSHOW:
			err = cmdsys_penstate (1);
			break;
		case CMDSYS_BPTOGGLE:
			err = cmdsys_bptoggle (lex, posstr);
			break;
		case CMDSYS_BPCLEAR:
			err = cmdsys_bpclear ();
			break;
	}
	return err;
}

void editstring_call (char *str, int *pos)
{
	if (mode_ed == MODE_EDIT) {
		get_cmdstr (str, get_cmd (cur_string));
		if (program_error)
			*pos = program_errpos;
		else
			*pos = strlen (str);
	}
}

int menu_handler (int cmd, int *key, int *curpos, char *str)
{
	int err = 0, l, r;
	char s[MAX_TOPLEN+1] = "";

	switch (cmd) {
		case CMDSYS_QUIT:
			err = cmdsys_quit ();
			if (!err) *key = K_ENTER;
			break;
		case CMDSYS_INSERT:
			err = dialog_curstring (MODE_INSERT, &l);
			if (!err)
				err = tracking (l);
			break;
		case CMDSYS_CHANGE:
			err = dialog_curstring (MODE_CHANGE, &l);
			if (!err)
				err = tracking (l);
			break;
		case CMDSYS_EDIT:
			err = dialog_curstring (MODE_EDIT, &l);
			if (!err)
				err = tracking (l);
			break;
		case CMDSYS_NEW:
			err = cmdsys_new ();
			break;
		case CMDSYS_LINES:
			err = cmdsys_lines ();
			break;
		case CMDSYS_LIST:
			err = listing (1, Buf_Sum_Str ());
			break;
		case CMDSYS_LOAD:
			err = cmdsys_load ();
			break;
		case CMDSYS_SAVE:
			err = cmdsys_save ();
			break;
		case CMDSYS_SAVEAS:
			err = cmdsys_saveas ();
			break;
		case CMDSYS_RUN:
			err = cmdsys_run ();
			break;
		case CMDSYS_PENSHOW:
			err = cmdsys_penstate (!pen_state);
			break;
		case CMDSYS_ABOUT:
			err = about_dialog ();
			break;
		case CMDSYS_CONTENTS:
			err = help ("CONTENTS");
			break;
		case CMDSYS_PREVTOPIC:
			err = help_prevtopic ();
			break;
		case CMDSYS_INDEX:
			err = help ("INDEX");
			break;
		case CMDSYS_TOPSEARCH:
			l = *curpos;
			while (str [l] != ' ' && str [l] != ',' && l >= 0)
				l--;
			l++;
			r = l;
			while (str [r] != ' ' && str [r] != ',' && str [r] && r-l < MAX_TOPLEN)
				r++;
			r--;
			strncat (s, str + l, r - l + 1);
			err = help (s);
			break;
	}
	editstring_call (str, curpos);
	return err;
}

void preedit (char *str, int *curpos, int cmd)
{
	strcpy (str, getname (cmd));
	if (getminpar (cmd))
		strcat (str, " ");
	*curpos = strlen (str);
}

void editfunc (int *key, int *curpos, char *str)
{
	int cmd = 0, err;

	switch (*key) {
		case K_ALTX:	cmd = CMDSYS_QUIT;			break;
		case K_F(1):	cmd = CMDSYS_CONTENTS;			break;
		case K_SF(1):	cmd = CMDSYS_INDEX;			break;
		case K_CF(1):	cmd = CMDSYS_TOPSEARCH;			break;
		case K_AF(1):	cmd = CMDSYS_PREVTOPIC;			break;
		case K_F(2):	cmd = CMDSYS_SAVE;			break;
		case K_F(3):	cmd = CMDSYS_LOAD;			break;
		case K_F(4):	cmd = CMDSYS_PENSHOW;			break;
		case K_F(5):	cmd = CMDSYS_EDIT;			break;
		case K_F(6):	cmd = CMDSYS_CHANGE;			break;
		case K_F(7):	cmd = CMDSYS_INSERT;			break;
		case K_F(8):	cmd = CMDSYS_LIST;			break;
		case K_F(9):	cmd = CMDSYS_RUN;			break;
		case K_F(10):	cmd = menu ();				break;
	}
	if (cmd) {
		err = menu_handler (cmd, key, curpos, str);
		if (err && err != ERR_CANCEL)
			error_con (err, -1, 0);
	}
	cmd = 0;
	switch ((char)*key) {
		case 'F':	cmd = CMD_FWD;		break;
		case 'B':	cmd = CMD_BWD;		break;
		case 'P':	cmd = CMD_REPEAT;	break;
		case 'O':	cmd = CMD_PROC;		break;
		case 'T':	cmd = CMD_RETURN;	break;
		case 'E':	cmd = CMD_END;		break;
		case 'S':	cmd = CMD_ELLIPSE;	break;
		case 'V':	cmd = CMD_PCOLOR;	break;
		case 'C':	cmd = CMD_BCOLOR;	break;
		case 'U':	cmd = CMD_PENUP;	break;
		case 'D':	cmd = CMD_PENDOWN;	break;
		case 'L':	cmd = CMD_LEFT;		break;
		case 'R':	cmd = CMD_RIGHT;	break;
		case 'A':	cmd = CMD_ANGLE;	break;
		case 'I':	cmd = CMD_FILL;		break;
		case 'X':	cmd = CMD_SETX;		break;
		case 'Y':	cmd = CMD_SETY;		break;
		case 'G':	cmd = CMD_SETXY;	break;
		case 'M':	cmd = CMD_CALL;		break;
		case 'W':	cmd = CMD_CLS;		break;
	}
	if (cmd) preedit (str, curpos, cmd);
	cur_pos = *curpos;
	status_refresh ();
}

void monitor_init ()
{
	int	i;

	opengraph ();
	setfillstyle (SOLID_FILL, DARKGRAY);
	bar (0, 0, SCREEN_X, SCREEN_Y);
	setcolor (CYAN);
	for (i = 0; i < 480; i += 5)
		line (0, i, SCREEN_X, i);
	for (i = 0; i < 640; i += 5)
		line (i, 0, i, SCREEN_Y);
	randomize ();
	file_init ();
	menu_init ();
	status_init ();
	drawframe (VIEW_X1, VIEW_Y1, VIEW_X2, VIEW_Y2);
	inter_init (VIEW_X1, VIEW_Y1, VIEW_X2, VIEW_Y2);
	console_init (CON_X, CON_Y, CON_VSYM, CON_MSYM, CON_HGT, editfunc);
	cmdsys_new ();
	about_dialog ();
}

void monitor_run ()
{
	char	c [CON_MSYM+2] = "", s [CON_MSYM+2];
	Lexem	lex [MAX_NUM_LEXEM];
	int	posstr = 0, err;

	err = help_init (HELP_FILENAME);
	if (_argc == 2 && !err) {
		err = program_load (_argv [1]);
		editstring_call (c, &posstr);
	}
	if (err)
		error_con (err, -1, 0);
	do {
		cur_pos = posstr;
		status_refresh ();
		console_edit (c, &posstr);

		if (program_quit)
			continue;

		error_cont (rasbiv (c, lex, &posstr));
		if (lex -> type != LEX_ENDSTR) {
			error_cont (check (lex, &posstr))
			if (lex -> value > CMD_INTER) {
				if (mode_ed == MODE_INSERT)
					error_cont (ins_cmd (lex, cur_string))
				else
					error_cont (chg_cmd (lex, cur_string))
				program_saved	= 0;
				console_message (list (s, cur_string, 0), CON_TXT_COLOR);
				error_cont (tracking (cur_string + 1))
			} else {
				console_message (get_cmdstr (s, lex), CON_TXT_COLOR);
				error_cont (inter_syscmd (lex, &posstr))
			}
		} else
			console_message (c, CON_TXT_COLOR);
		posstr	= 0;
		while (c [posstr] == ' ')
			posstr++;
		c [posstr] = 0;
		editstring_call (c, &posstr);
	} while (!program_quit);
}

void monitor_done ()
{
	help_done ();
	lex_done ();
	buf_init ();
	file_done ();
	closegraph ();
	printf ("Thank you for using this program.\n");
}

int main ()
{
	monitor_init ();
	monitor_run ();
	monitor_done ();
	return 0;
}
