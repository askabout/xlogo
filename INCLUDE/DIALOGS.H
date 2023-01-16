#ifndef __DIALOGS__
#define __DIALOGS__

#define DLG_MAXBUTTONS	10
#define DLG_MARGIN	2
#define DLG_NOACTBTN	DARKGRAY
#define DLG_ACTBTN	LIGHTGREEN

#define DLG_YES		1
#define DLG_NO		2
#define DLG_OK		3
#define DLG_CANCEL	4

void	dialog_init (int, int, int, int, const char *);
void	dialog_statictext (int, int, int, const char *);
void	dialog_button (int, int, int, int, const char *);
void	dialog_done ();
int	dialog_run ();

#endif