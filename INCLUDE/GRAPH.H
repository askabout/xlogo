#ifndef __GRAPH__
#define __GRAPH__

#define VIDEO_DRIVER		VGA
#define VIDEO_MODE		VGAHI

#define WIN_UPCOLOR		WHITE
#define WIN_MIDCOLOR		LIGHTGRAY
#define WIN_DOWNCOLOR		DARKGRAY
#define WIN_HDRCOLOR		RED

#define WIN_HDRHGT		10
#define MAX_WINDOWS		20
#define MAX_SETSAVED 		20

void		opengraph ();
void		waitretrace ();
void		drawframe (int, int, int, int);
void		drawhwindow (int, int, int, int, const char *);
void		drawwindow (int, int, int, int);
void		openhwindow (int, int, int, int, const char *);
void		openwindow (int, int, int, int);
void		closewindow ();
void		drawupbutton (int, int, int, int);
void		drawdownbutton (int, int, int, int);
void		drawpanel (int, int, int, int);
void		drawupline (int, int, int, int);
void		drawdownline (int, int, int, int);
void		save_settings ();
void		load_settings ();

#endif