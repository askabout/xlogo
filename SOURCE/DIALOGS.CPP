#include <bios.h>
#include <string.h>
#include <keyboard.h>
#include <graphics.h>

#include "dialogs.h"
#include "graph.h"

struct Button {
	int c, x, y, l;
	char s [80];
};

static int	dlg_x1, dlg_y1, dlg_x2, dlg_y2, dlg_nb;
static Button	but [DLG_MAXBUTTONS];

void dialog_init (int x1, int y1, int x2, int y2, const char *hdr)
{
	openhwindow (x1, y1, x2, y2, hdr);
	dlg_x1	= x1;
	dlg_y1	= y1;
	dlg_x2	= x2;
	dlg_y2	= y2;
	dlg_nb	= 0;
}

void dialog_statictext (int x, int y, int c, const char *s)
{
	save_settings ();
	setcolor (c);
	outtextxy (dlg_x1 + x, dlg_y1 + y, s);
	load_settings ();
}

void draw_button (int n, int stat)
{
	save_settings ();
	settextjustify (CENTER_TEXT, TOP_TEXT);
	setcolor (stat ? DLG_ACTBTN : DLG_NOACTBTN);
	outtextxy (but [n].x + (but [n].l << 2), but [n].y, but [n].s);
	load_settings ();
}

void dialog_button (int c, int x, int y, int l, const char *s)
{
	save_settings ();
	waitretrace ();
	x += dlg_x1;
	y += dlg_y1;
	drawupbutton (x - DLG_MARGIN, y - DLG_MARGIN, x + (l << 3) + DLG_MARGIN, y + 8 + DLG_MARGIN);
	but [dlg_nb].c	= c;
	but [dlg_nb].x	= x;
	but [dlg_nb].y	= y;
	but [dlg_nb].l	= l;
	strcpy (but [dlg_nb].s, s);
	draw_button (dlg_nb, 0);
	dlg_nb++;
	load_settings ();
}

void dialog_done ()
{
	closewindow ();
}

int dialog_run ()
{
	int	i = 0, key;

	do {
		draw_button (i, 1);
		key = bioskey (_KEYBRD_READ);
		draw_button (i, 0);
		switch (key) {
			case K_LEFTARROW:
			case K_UPARROW:
			case K_STAB:
				i = i > 0 ? i - 1 : dlg_nb - 1;
				break;
			case K_RIGHTARROW:
			case K_DOWNARROW:
			case K_TAB:
				i = i < dlg_nb - 1 ? i + 1 : 0;
				break;
			case K_PAGEUP:
			case K_HOME:
				i = 0;
				break;
			case K_PAGEDOWN:
			case K_END:
				i = dlg_nb - 1;
				break;
		}
	} while (key != K_ENTER && key != K_ESCAPE);
	if (key == K_ESCAPE)
		return DLG_CANCEL;
	return but [i].c;
}
