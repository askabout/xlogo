#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <graphics.h>

#include "status.h"
#include "graph.h"
#include "xlogo.h"
#include "inter.h"

char	o_nam [80];
int	o_str, o_pos, o_mod, o_sav, o_ps;
int	o_clr, o_pen, o_x, o_y, o_ang;

void status_init ()
{
	save_settings ();
	drawpanel (1, STATUS_Y, getmaxx () - 1, STATUS_Y + STATUS_HGT - 1);
	drawdownbutton (MARGIN_CLR - 4, STATUS_SY - 2, MARGIN_CLR + 3, STATUS_SY + 5);
	setfillstyle (SOLID_FILL, BLACK);
	bar (MARGIN_CLR - 4, STATUS_SY - 2, MARGIN_CLR + 3, STATUS_SY + 5);
	drawdownbutton (MARGIN_CLR, STATUS_SY + 2, MARGIN_CLR + 7, STATUS_SY + 9);
	load_settings ();
	*o_nam	= 0;
	o_str	= -1;
	o_pos	= -1;
	o_mod	= -1;
	o_clr	= -1;
	o_pen	= -1;
	o_x	= -1;
	o_y	= -1;
	o_ang	= -1;
	o_ps	= -1;
}

void status_erase (int margin, int width)
{
	bar ((margin << 3) + 4, STATUS_SY, ((margin + width) << 3) + 4, STATUS_SY + 7);
}

void status_print (int margin, char *s)
{
	outtextxy ((margin << 3) + 4, STATUS_SY, s);
}

void status_refresh ()
{
	char	nam [80], s[50], *t;
	int	str, pos, mod, sav, ps;
	int	clr, pen, x, y, ang;

	save_settings ();
	get_sys_state (nam, &sav, &mod, &str, &pos, &ps);
	get_inter_stat (&clr, &pen, &x, &y, &ang);
	setfillstyle (SOLID_FILL, LIGHTGRAY);
	waitretrace ();
	if (pos != o_pos || str != o_str) {
		setcolor (WHITE);
		status_erase (MARGIN_POS, 7);
		sprintf (s, "%3d:%d", str, pos);
		status_print (MARGIN_POS, s);
	}
	if (sav != o_sav) {
		status_erase (MARGIN_SAV, 1);
		setcolor (LIGHTRED);
		if (!sav)
			status_print (MARGIN_SAV, "*");
	}
	if (mod != o_mod) {
		status_erase (MARGIN_MOD, 7);
		switch (mod) {
			case MODE_INSERT:
				setcolor (YELLOW);
				t = "ВСТАВКА";
				break;
			case MODE_CHANGE:
				setcolor (LIGHTRED);
				t = " ЗАМЕНА";
				break;
			case MODE_EDIT:
				setcolor (LIGHTGREEN);
				t = " РЕДАКТ";
				break;
		}
		status_print (MARGIN_MOD, t);
	}
	if (strcmp (o_nam, nam)) {
		setcolor (WHITE);
		status_erase (MARGIN_SAV - strlen (o_nam) - 1, strlen (o_nam));
		status_print (MARGIN_SAV - strlen (nam) - 1, nam);
	}
	if (x != o_x || y != o_y) {
		setcolor (WHITE);
		status_erase (MARGIN_XY, 7);
		sprintf (s, "%3d,%d", x, y);
		status_print (MARGIN_XY, s);
	}
	if (ang != o_ang) {
		setcolor (WHITE);
		status_erase (MARGIN_ANG, 4);
		sprintf (s, "%3d°", ang);
		status_print (MARGIN_ANG, s);
	}
	if (pen != o_pen || ps != o_ps) {
		setcolor (LIGHTGREEN);
		status_erase (MARGIN_PEN, 1);
		if (ps)
			status_print (MARGIN_PEN, pen ? "\x1F" : "\x1E");
		else
			status_print (MARGIN_PEN, pen ? "\x19" : "\x18");
	}
	if (clr != o_clr) {
		setfillstyle (SOLID_FILL, clr);
		bar (MARGIN_CLR, STATUS_SY + 2, MARGIN_CLR + 7, STATUS_SY + 9);
	}
	o_pos	= pos;
	o_str	= str;
	o_sav	= sav;
	o_mod	= mod;
	o_clr	= clr;
	o_pen	= pen;
	o_x	= x;
	o_y	= y;
	o_ps	= ps;
	strcpy (o_nam, nam);
	load_settings ();
}
