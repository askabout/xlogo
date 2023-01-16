#include <graphics.h>
#include <stdio.h>
#include <process.h>
#include <alloc.h>

#include "graph.h"
#include "error.h"

struct Window {
	int	x, y;
	void	*savepic;
};

struct Settings {
	int			c;
	fillsettingstype	fs;
	textsettingstype	ts;
};

static Settings		set [MAX_SETSAVED];
static int		setpos;

static Window		windows [MAX_WINDOWS];
static int		nwin;

// Инициация графики

void opengraph ()
{
	int gdriver	= VIDEO_DRIVER;
	int gmode	= VIDEO_MODE;
	int err;

	err = registerbgidriver (EGAVGA_driver);
	if (err > 0) {
		initgraph (&gdriver, &gmode, "");
		err = graphresult ();
	}
	if (err != grOk)
		error_fatal (grapherrormsg (err));
}

// Ждет начала прорисовки экрана

void waitretrace ()
{
/*
	asm mov	dx, 0x03DA
	Loop1: asm {
		in	al, dx
		and	al, 8
		jz	Loop1
	}
	Loop2: asm {
		in	al, dx
		and	al, 8
		jnz	Loop2
	}
*/
}

// Рисует рамку окна внешним образом

void drawframe (int x1, int y1, int x2, int y2)
{
	save_settings ();
	setcolor (WIN_DOWNCOLOR);
	rectangle (x1 - 1, y1 - 1, x2 + 1, y2 + 1);
	rectangle (x1 - 5, y1 - 5, x2 + 5, y2 + 5);
	setcolor (WIN_MIDCOLOR);
	rectangle (x1 - 2, y1 - 2, x2 + 2, y2 + 2);
	rectangle (x1 - 4, y1 - 4, x2 + 4, y2 + 4);
	setcolor (WIN_UPCOLOR);
	rectangle (x1 - 3, y1 - 3, x2 + 3, y2 + 3);
	load_settings ();
}

// Рисует окно внешним образом с заголовком

void drawhwindow (int x1, int y1, int x2, int y2, const char *hdr)
{
	int i, l, tx, tx1, tx2, ty1 = y1 - WIN_HDRHGT;

	save_settings ();
	setcolor (WIN_DOWNCOLOR);
	rectangle (x1 - 1, ty1 - 1, x2 + 3, y2 + 3);
	line (x1 - 1, y1 - 1, x2 + 3, y1 - 1);
	setcolor (WIN_MIDCOLOR);
	rectangle (x1 - 2, ty1 - 2, x2 + 2, y2 + 2);
	setcolor (WIN_UPCOLOR);
	rectangle (x1 - 3, ty1 - 3, x2 + 1, y2 + 1);
	setfillstyle (SOLID_FILL, WIN_MIDCOLOR);
	bar (x1 - 1, ty1 - 2, x2 + 1, y1 - 2);
	bar (x1, y1, x2, y2);
	l   = textwidth (hdr);
	tx  = (x1 + x2) / 2;
	tx1 = tx - l / 2;
	tx2 = tx + l / 2;
	for (i = ty1 - 1; i < y1 - 3; i += 3) {
		setcolor (WIN_DOWNCOLOR);
		line (x1 + 5, i, tx1 - 5, i);
		line (tx2 + 5, i, x2 - 5, i);
		setcolor (WIN_UPCOLOR);
		line (x1 + 5, i + 1, tx1 - 5, i + 1);
		line (tx2 + 5, i + 1, x2 - 5, i + 1);
	}
	setcolor (WIN_HDRCOLOR);
	outtextxy (tx1, ty1, hdr);
	load_settings ();
}

void drawwindow (int x1, int y1, int x2, int y2)
{
	save_settings ();
	setcolor (WIN_DOWNCOLOR);
	rectangle (x1 - 1, y1 - 1, x2 + 3, y2 + 3);
	line (x1 - 1, y1 - 1, x2 + 3, y1 - 1);
	setcolor (WIN_MIDCOLOR);
	rectangle (x1 - 2, y1 - 2, x2 + 2, y2 + 2);
	setcolor (WIN_UPCOLOR);
	rectangle (x1 - 3, y1 - 3, x2 + 1, y2 + 1);
	setfillstyle (SOLID_FILL, WIN_MIDCOLOR);
	bar (x1, y1, x2, y2);
	load_settings ();
}

void openwindow (int x1, int y1, int x2, int y2)
{
	int	tx1 = x1 - 3;
	int	ty1 = y1 - 3;
	int	tx2 = x2 + 3;
	int	ty2 = y2 + 3;
	windows [nwin].x	= tx1;
	windows [nwin].y	= ty1;
	windows [nwin].savepic	= farmalloc (imagesize (tx1, ty1, tx2, ty2));
	getimage (tx1, ty1, tx2, ty2, windows [nwin].savepic);
	drawwindow (x1, y1, x2, y2);
	nwin++;
}

void openhwindow (int x1, int y1, int x2, int y2, const char *hdr)
{
	int	tx1 = x1 - 3;
	int	ty1 = y1 - WIN_HDRHGT - 3;
	int	tx2 = x2 + 3;
	int	ty2 = y2 + 3;
	windows [nwin].x	= tx1;
	windows [nwin].y	= ty1;
	if (!(windows [nwin].savepic = farmalloc (imagesize (tx1, ty1, tx2, ty2))))
		error_fatal ("Can't allocate memory for saving image");
	getimage (tx1, ty1, tx2, ty2, windows [nwin].savepic);
	drawhwindow (x1, y1, x2, y2, hdr);
	nwin++;
}

void closewindow ()
{
	if (nwin > 0) {
		nwin--;
		putimage (windows [nwin].x, windows [nwin].y, windows [nwin].savepic, COPY_PUT);
		farfree (windows [nwin].savepic);
	} else
        	error_fatal ("Extra closing window");
}

void drawupbutton (int x1, int y1, int x2, int y2)
{
	save_settings ();
	setcolor (WIN_UPCOLOR);
	line (x1 - 1, y1 - 1, x2 + 1, y1 - 1);
	line (x1 - 1, y1 - 1, x1 - 1, y2 + 1);
	setcolor (WIN_DOWNCOLOR);
	line (x2 + 1, y2 + 1, x1 - 1, y2 + 1);
	line (x2 + 1, y2 + 1, x2 + 1, y1 - 1);
	load_settings ();
}

void drawdownbutton (int x1, int y1, int x2, int y2)
{
	save_settings ();
	setcolor (WIN_UPCOLOR);
	line (x2 + 1, y2 + 1, x1 - 1, y2 + 1);
	line (x2 + 1, y2 + 1, x2 + 1, y1 - 1);
	setcolor (WIN_DOWNCOLOR);
	line (x1 - 1, y1 - 1, x2 + 1, y1 - 1);
	line (x1 - 1, y1 - 1, x1 - 1, y2 + 1);
	load_settings ();
}

void drawpanel (int x1, int y1, int x2, int y2)
{
	save_settings ();
	drawupbutton (x1, y1, x2, y2);
	setfillstyle (SOLID_FILL, WIN_MIDCOLOR);
	bar (x1, y1, x2, y2);
	load_settings ();
}

void drawupline (int x1, int y1, int x2, int y2)
{
	save_settings ();
	setcolor (WIN_UPCOLOR);
	line (x1, y1, x2, y2);
	setcolor (WIN_DOWNCOLOR);
	line (x1 + 1, y1 + 1, x2 + 1, y2 + 1);
	load_settings ();
}

void drawdownline (int x1, int y1, int x2, int y2)
{
	save_settings ();
	setcolor (WIN_UPCOLOR);
	line (x1 + 1, y1 + 1, x2 + 1, y2 + 1);
	setcolor (WIN_DOWNCOLOR);
	line (x1, y1, x2, y2);
	load_settings ();
}

void save_settings ()
{
	set [setpos].c = getcolor ();
	getfillsettings (&set [setpos].fs);
	gettextsettings (&set [setpos].ts);
	setpos++;
        if (setpos > MAX_SETSAVED)
                error_fatal ("No more room to save graphic settings");
}

void load_settings ()
{
	if (setpos > 0) {
		setpos--;
		setcolor (set [setpos].c);
		setfillstyle (set [setpos].fs.pattern, set [setpos].fs.color);
		settextstyle (set [setpos].ts.font, set [setpos].ts.direction, set [setpos].ts.charsize);
		settextjustify (set [setpos].ts.horiz, set [setpos].ts.vert);
	} else
        	error_fatal ("Extra loading of graphic settings");
}
