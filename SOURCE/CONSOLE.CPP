#include <graphics.h>
#include <string.h>

#include "console.h"
#include "edit.h"
#include "graph.h"
#include "keyboard.h"

struct MessageBuffer {
	int	color;
	char    msg [EDIT_BUFFERWIDTH];
};

static EditSettings	es;
static MessageBuffer	conbuf [CON_BUFHGT];
static int		conx, cony, conwdt, conhgt, conbufpos;

static void (*usrfunc)(int *, int *, char *);

void console_refresh ()
{
	int cony2 = cony + ((conhgt - 1) << 3), i, j;

	save_settings ();
	setfillstyle (SOLID_FILL, CON_BGCOLOR);
	settextstyle (DEFAULT_FONT, HORIZ_DIR, 1);
	setcolor (CON_PTRCOLOR);
	waitretrace ();
	bar (conx, cony, conx + (conwdt << 3), cony2 + 8);
	outtextxy (conx, cony2, ">");
	for (i = 1, j = conbufpos; i < conhgt; i++, j--) {
		setcolor (conbuf [j].color);
		outtextxy (conx, cony2 - (i << 3), conbuf [j].msg);
	}
	load_settings ();
}

void console_editfunc (int *key, int *curpos, char *str)
{
	switch (*key) {
		case K_PAGEUP:
			if (conbufpos > conhgt - 2) {
				conbufpos--;
				console_refresh ();
			}
			break;
		case K_PAGEDOWN:
			if (conbufpos < CON_BUFHGT - 1) {
				conbufpos++;
				console_refresh ();
			}
			break;
	}
	usrfunc (key, curpos, str);
}

void console_init (int x, int y, int width, int max, int height, void (*fn)(int *, int *, char *))
{
	conx 	= x;
	cony	= y;
	conwdt	= width;
	conhgt	= height;
	usrfunc	= fn;
	drawframe (x, y, x + (width << 3), y + (height << 3));
	edit_init (&es, x + 8, y + (height - 1 << 3), width - 1, max, CON_BGCOLOR, CON_COLOR, console_editfunc);
	for (int i = 0; i < CON_BUFHGT; i++)
		*conbuf [i].msg = '\0';
	console_refresh ();
}

void console_message (char *str, int color)
{
	int slen = 0;

	conbufpos = CON_BUFHGT - 1;
	do {
		for (int i = 1; i <= conbufpos; i++) {
			strcpy (conbuf [i - 1].msg, conbuf [i].msg);
			conbuf [i - 1].color = conbuf [i].color;
		}
		strncpy (conbuf [conbufpos].msg, str + slen, conwdt);
		conbuf [conbufpos].color = color;
		slen += conwdt;
	} while (slen < strlen (str));
	console_refresh ();
}

void console_edit (char *str, int *curpos)
{
	edit (&es, str, curpos);
}
