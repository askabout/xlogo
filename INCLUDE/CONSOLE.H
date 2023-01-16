#ifndef __EDITOR__
#define __EDITOR__

#define CON_BGCOLOR		BLUE
#define CON_PTRCOLOR		LIGHTGREEN
#define CON_COLOR		WHITE

#define CON_BUFHGT		30

void console_init (int, int, int, int, int, void (*)(int *, int *, char *));
void console_message (char *, int);
void console_edit (char *, int *);

#endif