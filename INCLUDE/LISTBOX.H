#ifndef __LISTBOX__
#define __LISTBOX__

#define LISTBOX_MAXITEMS	256
#define LISTBOX_BGCOLOR		CYAN
#define LISTBOX_CBGCOLOR	GREEN

void	listbox_init (int, int, int, int, void (*) (int *), int (*) (void *, void *));
void	listbox_draw (int);
void	listbox_done ();
void	listbox_add (void *, size_t, int);
int	listbox_run (int *, void *);

#endif