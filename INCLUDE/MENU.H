#ifndef __MENU__
#define __MENU__

#define MENU_SELCOLOR		GREEN
#define MENU_ITEMCOLOR		WHITE
#define MENU_MAXNAME		30
#define MENU_MAXITEMS		10
#define MENU_MAX		10
#define MENU_MARGIN		10
#define MENU_ITEMX              8
#define MENU_ITEMY		4
#define MENU_ITEMHGT		((MENU_ITEMY << 1) + 8)
#define MENU_X			0
#define MENU_Y			(MENU_ITEMHGT + 6)

int	menu_add (char *);
void	menu_additem (int, char *, int);
void	menubar_draw ();
int	menu ();

#endif