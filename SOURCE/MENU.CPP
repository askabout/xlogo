#include <string.h>
#include <graphics.h>
#include <bios.h>

#include "menu.h"
#include "graph.h"
#include "keyboard.h"

struct MenuItem {
	char	name [MENU_MAXNAME];
	int	code;
};

struct Menu {
	char		name [MENU_MAXNAME];
	int		x, wdt, curitem, itemwdt, nitems;
	MenuItem	items [MENU_MAXITEMS];
};

static Menu	menus [MENU_MAX];
static int	curmenu, nmenus, curx = MENU_MARGIN;

int item_wdt (char *name)
{
	return ((strlen (name) << 3) + (MENU_ITEMX << 1));
}

int menu_add (char *name)
{
	Menu *menu = &menus [nmenus];

	strcpy (menu -> name, name);
	menu -> x	= curx;
	menu -> wdt	= item_wdt (name);
	menu -> curitem	= 0;
	menu -> nitems	= 0;
	menu -> itemwdt = menu -> wdt;
	curx		+= menu -> wdt;
	return (nmenus++);
}

void menu_additem (int menu, char *name, int code)
{
	Menu *pmenu	= &menus [menu];
	MenuItem *item	= &pmenu -> items [pmenu -> nitems++];
	int wdt         = item_wdt (name);

	strcpy (item -> name, name);
	item -> code = code;
	if (wdt > pmenu -> itemwdt)
		pmenu -> itemwdt = wdt;
}

void item_draw (int x, int y, int wdt, char *item, int sel)
{
	save_settings ();
	setfillstyle (SOLID_FILL, sel ? MENU_SELCOLOR : WIN_MIDCOLOR);
	waitretrace ();
	setcolor (MENU_ITEMCOLOR);
	bar (x, y, x + wdt, y + MENU_ITEMHGT - 1);
	outtextxy (x + MENU_ITEMX, y + MENU_ITEMY, item);
	load_settings ();
}

void menu_drawitem (int menu, int item, int sel)
{
	Menu *pmenu	= &menus [menu];
	MenuItem *pitem	= &pmenu -> items [item];
	int x		= MENU_X + pmenu -> x;
	int dx		= MENU_ITEMX / 2;
	int y		= MENU_Y + item * MENU_ITEMHGT;
	int y1		= y + MENU_ITEMHGT / 2 - 1;

	if (pitem -> code)
		item_draw (x, y, pmenu -> itemwdt, pitem -> name, sel);
	else
		drawdownline (x + dx, y1, x + pmenu -> itemwdt - dx, y1);
}

void menu_draw (int menu)
{
	int x = MENU_X + menus [menu].x;
	int y = MENU_Y + menus [menu].nitems * MENU_ITEMHGT;

	openwindow (x, MENU_Y, x + menus [menu].itemwdt, y);
	for (int i = 0; i < menus [menu].nitems; i++)
		menu_drawitem (menu, i, i == menus [menu].curitem);
}

void menubar_drawitem (int menu, int sel)
{
	item_draw (menus [menu].x, 1, menus [menu].wdt, menus [menu].name, sel);
}

void menubar_draw ()
{
	drawpanel (1, 1, getmaxx () - 1, MENU_ITEMHGT);
	for (int i = 0; i < nmenus; i++)
		menubar_drawitem (i, 0);
}

void menubar_next (int dm)
{
	closewindow ();
	menubar_drawitem (curmenu, 0);
	curmenu += dm;
	if (curmenu >= nmenus)
		curmenu = 0;
	if (curmenu < 0)
		curmenu = nmenus - 1;
	menubar_drawitem (curmenu, 1);
	menu_draw (curmenu);
}

void menu_next (int dm)
{
	int *curitem	= &menus [curmenu].curitem;
	int nitems	= menus [curmenu].nitems;

	menu_drawitem (curmenu, *curitem, 0);
	while (!menus [curmenu].items [*curitem += dm].code);
	if (*curitem >= nitems)
		*curitem = 0;
	if (*curitem < 0)
		*curitem = nitems - 1;
	menu_drawitem (curmenu, *curitem, 1);
}

int menu ()
{
	int code = -1, key;

	menubar_drawitem (curmenu, 1);
	menu_draw (curmenu);
	do {
		key = 0;
		while (bioskey (_KEYBRD_READY))
			key = bioskey (_KEYBRD_READ);
		switch (key) {
			case K_LEFTARROW:
				menubar_next (-1);
				break;
			case K_RIGHTARROW:
				menubar_next (1);
				break;
			case K_UPARROW:
				menu_next (-1);
				break;
			case K_DOWNARROW:
				menu_next (1);
				break;
			case K_ENTER:
				code = menus [curmenu].items [menus [curmenu].curitem].code;
				break;
			case K_ESCAPE:
				code = 0;
				break;
			}
	} while (code < 0);
	closewindow ();
	menubar_drawitem (curmenu, 0);
	return (code);
}
