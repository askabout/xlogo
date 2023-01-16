#ifndef __EDIT__
#define __EDIT__

#define EDIT_BUFFERWIDTH	256
#define EDIT_BUFFERHEIGHT	10

#define CURSOR_BLINKDELAY	700
#define CURSOR_COLOR		YELLOW

struct EditSettings {
	int	x, y, vissym, maxsym;
	int	fgcolor, bgcolor;
	int	curpos;
	char	buffer [EDIT_BUFFERHEIGHT][EDIT_BUFFERWIDTH];
	void	(*editfunc)(int *, int *, char *);
};

char	*strdel (char *, int, int);
char	*strins (char *, char *, int);
void	edit_init (EditSettings *, int, int, int, int, int, int, void (*)(int *, int *, char *));
void	edit_draw (EditSettings *, char *, int);
void	edit (EditSettings *, char *, int *);

#endif __EDIT__