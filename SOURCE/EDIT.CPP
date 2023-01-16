#include <bios.h>
#include <string.h>
#include <graphics.h>
#include <ctype.h>
#include <dos.h>

#include "graph.h"
#include "edit.h"
#include "keyboard.h"

static int edit_insmode = 1;

/*
	Инициализация ввода в заданной позиции
	es      - пеpeменная, где хpанить установки
	x	- координата в пикселах
	y	- координата в пикселах
	vsym	- ширина строки ввода
	msym	- максимальная длина строки
	bc      - цвет фона редактируемой строки
	fc	- цвет редактируемой строки
	fn	- функция, вызываемая редактором при нажати клавиши
		  Паpaметpы:

		  *key		- код нажатой клавиши
		  *curpos	- текущая позиция куpcopа в cтpоке
		  *str		- peдактиpуемая стpoка
*/

void edit_init (EditSettings *es, int x, int y, int vsym, int msym, int bc, int fc, void (*fn)(int *, int *, char *))
{
	es -> x		= x;
	es -> y		= y;
	es -> vissym	= vsym;
	es -> maxsym	= msym;
	es -> bgcolor	= bc;
	es -> fgcolor	= fc;
	es -> editfunc	= fn;
	for (int i = 0; i < EDIT_BUFFERHEIGHT; i++)
		*es -> buffer [i] = '\0';
}

char *strdel (char *dest, int beg, int count)
{
	return (strcpy (dest+beg, dest+beg+count));
}

char *strins (char *dest, char *src, int pos)
{
	int	srclen = strlen (src);
	char	*destp = dest + pos;
	memmove (destp + srclen, destp, strlen (destp)+1);
	return ((char *)memcpy (destp, src, srclen));
}

void draw_cursor (EditSettings *es, int pos)
{
	int x = es -> x + (pos << 3);
	for (int j = edit_insmode ? 7 : 0; j < 8; j++)
		line (x, es -> y + j, x + 7, es -> y + j);
}

void edit_cursor (EditSettings *es, int pos)
{
	int p = 0, s = 0;

	save_settings ();
	setcolor (CURSOR_COLOR ^ es -> bgcolor);
	setwritemode (XOR_PUT);
	while (!bioskey (_KEYBRD_READY)) {
		if (!(p % CURSOR_BLINKDELAY)) {
			draw_cursor (es, pos);
			s = !s;
		}
		delay (1);
		p++;
	}
	if (s) draw_cursor (es, pos);
	setwritemode (COPY_PUT);
	load_settings ();
}

void edit_draw (EditSettings *es, char *str, int vp)
{
	char visbuf [EDIT_BUFFERWIDTH] = "";

	strncpy (visbuf, str + vp, es -> vissym);
	waitretrace ();
	bar (es -> x, es -> y, es -> x + (es -> vissym << 3), es -> y + 7);
	outtextxy (es -> x, es -> y, visbuf);
}

// Редактирование строки str, используя установки es

void edit (EditSettings *es, char *str, int *curpos)
{
	int	vispos = 0, t, key;
	char	newsym [2] = " ";
	int	bufpos = EDIT_BUFFERHEIGHT - 1;

	save_settings ();
	setcolor (es -> fgcolor);
	setfillstyle (SOLID_FILL, es -> bgcolor);
	settextstyle (DEFAULT_FONT, HORIZ_DIR, 1);
	settextjustify (LEFT_TEXT, TOP_TEXT);
	do {
		t = strlen (str) - es -> vissym + 1;
		if (vispos > t)
			vispos = (t > 0) * t;
		t = *curpos - es -> vissym + 1;
		if (vispos < t)
			vispos = t;
		if (vispos > *curpos)
			vispos = *curpos;
		edit_draw (es, str, vispos);
		edit_cursor (es, *curpos - vispos);
		key = bioskey (_KEYBRD_READ);
		switch (key) {
			case K_UPARROW:
				if (bufpos > 0)
					bufpos--;
				strcpy (str, es -> buffer [bufpos]);
				*curpos = strlen (str);
				break;
			case K_DOWNARROW:
				if (bufpos < EDIT_BUFFERHEIGHT - 1)
					bufpos++;
				strcpy (str, es -> buffer [bufpos]);
				*curpos = strlen (str);
				break;
			case K_LEFTARROW:
				if (*curpos > 0) (*curpos)--;
				break;
			case K_RIGHTARROW:
				if (*curpos < strlen (str)) (*curpos)++;
				break;
			case K_CLEFTARROW:
				(*curpos)--;
				while (*curpos > 0 && str [*curpos] == ' ' || str [*curpos] == ',') (*curpos)--;
				while (*curpos > 0 && str [*curpos] != ' ' && str [*curpos] != ',') (*curpos)--;
				if (*curpos) (*curpos)++;
				break;
			case K_CRIGHTARROW:
				while (*curpos < strlen (str) && str [*curpos] == ' ' || str [*curpos] == ',') (*curpos)++;
				while (*curpos < strlen (str) && str [*curpos] != ' ' && str [*curpos] != ',') (*curpos)++;
				if (*curpos != strlen (str)) (*curpos)++;
				break;
			case K_HOME:
				*curpos = 0;
				break;
			case K_END:
				*curpos = strlen (str);
				break;
			case K_DEL:
				strdel (str, *curpos, 1);
				break;
			case K_BACKSPACE:
				if (*curpos > 0) {
					(*curpos)--;
					strdel (str, *curpos, 1);
				}
				break;
			case K_INS:
				edit_insmode ^= 1;
				break;
			default:
				if ((unsigned char)key >= 0x20 && (unsigned char)key < 0x80) {
					*newsym = toupper (key);
					if (edit_insmode || *curpos == strlen (str)) {
						if (strlen (str) < es -> maxsym) {
							strins (str, newsym, *curpos);
							(*curpos)++;
						}
					} else
						if (*curpos < es -> maxsym) {
							str [*curpos] = *newsym;
							(*curpos)++;
						}
				}
		}
		if (es -> editfunc)
			es -> editfunc (&key, curpos, str);
	} while (key != K_ENTER);
	strcpy (es -> buffer [EDIT_BUFFERHEIGHT - 1], str);
	for (int i = 1; i < EDIT_BUFFERHEIGHT; i++)
		strcpy (es -> buffer [i - 1], es -> buffer [i]);
	*es -> buffer [EDIT_BUFFERHEIGHT - 1] = '\0';
	load_settings ();
}

