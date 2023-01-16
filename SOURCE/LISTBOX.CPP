#include <stdio.h>
#include <string.h>
#include <graphics.h>
#include <bios.h>
#include <alloc.h>

#include "listbox.h"
#include "graph.h"
#include "keyboard.h"
#include "error.h"

struct ListBox {
	void	*s;
	size_t	size;
	int	c;
	ListBox	*prev, *next;
};

static ListBox	*head, *tail, *pvis, *pcur;
static int	box_x, box_y, box_wdt, box_hgt, box_n, ovis, ox;
static int	vis, cur, cx;

static void	(*listfunc) (int *);
static int	(*sortfunc) (void *, void *);

void listbox_init (int x, int y, int wdt, int hgt, void (*lf) (int *), int (*sf) (void *, void *))
{
	box_x		= x;
	box_y		= y;
	box_wdt		= wdt;
	box_hgt		= hgt;
	listfunc	= lf;
	sortfunc	= sf;
	box_n		= 0;
	vis = cur = cx	= 0;
	ovis		= -1;
	head = tail	= NULL;
	drawdownbutton (x, y, x + (wdt << 3), y + (hgt << 3));
}

void listbox_done ()
{
	ListBox *p;
	while (head) {
		p = head -> next;
		free (head -> s);
		delete head;
		head = p;
	}
}

void listbox_add (void *s, size_t size, int c)
{
	ListBox *x, *n = head, *p = NULL;
	if (!(x = new ListBox))
		error_fatal ("Can't allocate memory for listbox item");
	if (!(x -> s = malloc (size)))
		error_fatal ("Can't allocate memory for listbox data");
	memcpy (x -> s, s, size);
	x -> c		= c;
	x -> size	= size;
	while (n && (!sortfunc || sortfunc (x -> s, n -> s) > 0)) {
		p = n;
		n = n -> next;
	}
	x -> next = n;
	x -> prev = p;
	if (p) p -> next = x;
	else head = x;
	if (n) n -> prev = x;
	else tail = x;
	pvis = pcur = head;
	box_n++;
}

void listbox_item (ListBox *p, int y, int sel)
{
	char	*s = (char *)p -> s, *c, t;

	save_settings ();
	setfillstyle (SOLID_FILL, sel ? LISTBOX_CBGCOLOR : LISTBOX_BGCOLOR);
	bar (box_x, box_y + (y << 3), box_x + (box_wdt << 3), box_y + (y << 3) + 7);
	if (vis + y < box_n) {
		if (cx + box_wdt < strlen (s))
			c = &s [cx + box_wdt];
		t	= *c;
		*c	= 0;
		setcolor (p -> c);
		if (cx < strlen (s))
			outtextxy (box_x, box_y + (y << 3), &s [cx]);
		*c	= t;
	}
	load_settings ();
}

void listbox_draw (int sel)
{
	ListBox *t = pvis;
	if (vis != ovis || ox != cx) {
		waitretrace ();
		for (int i = 0; i < box_hgt; i++) {
			listbox_item (t, i, vis + i == cur && sel);
			t = t -> next;
		}
	} else listbox_item (pcur, cur - vis, sel);
	ovis	= vis;
	ox	= cx;
}

int listbox_run (int *pos, void *s)
{
	int	key = 0, j = *pos, mx;

	pcur = pvis = head;
	do {
		while (j != 0)
			if (j > 0) {
				if (cur < box_n - 1) {
					cur++;
					pcur = pcur -> next;
					if (cur - vis >= box_hgt) {
						vis++;
						pvis = pvis -> next;
					}
				}
				j--;
			} else {
				if (cur > 0) {
					cur--;
					pcur = pcur -> prev;
					if (cur - vis < 0) {
						vis--;
						pvis = pvis -> prev;
					}
				}
				j++;
			}
		listbox_draw (1);
		key	= bioskey (_KEYBRD_READ);
		mx      = (int)strlen ((char *)pcur -> s) - box_wdt;
		switch (key) {
			case K_PAGEUP:
				j = -box_hgt + 1;
				break;
			case K_UPARROW:
				j = -1;
				break;
			case K_PAGEDOWN:
				j = box_hgt - 1;
				break;
			case K_DOWNARROW:
				j = 1;
				break;
			case K_LEFTARROW:
				if (cx > 0)
					cx--;
				break;
			case K_RIGHTARROW:
				if (cx < mx)
					cx++;
				break;
			case K_HOME:
				cx = 0;
				break;
			case K_END:
				cx = mx < 0 ? 0 : mx;
				break;
		}
		waitretrace ();
		listbox_item (pcur, cur - vis, 0);
		if (listfunc)
			listfunc (&key);
	} while (key != K_ENTER && key != K_ESCAPE);
	if (key == K_ESCAPE)
		return 1;
	*pos	= cur;
	memcpy (s, pcur -> s, pcur -> size);
	return 0;
}
