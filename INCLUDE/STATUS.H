#ifndef __STATUS__
#define __STATUS__

#define MODE_INSERT	0
#define MODE_CHANGE	1
#define MODE_EDIT	2

#define STATUS_MARGINY	4
#define STATUS_HGT	((STATUS_MARGINY << 1) + 8)
#define STATUS_Y	(480 - STATUS_HGT)
#define	STATUS_SY	(STATUS_Y + STATUS_MARGINY)

#define MARGIN_POS	72
#define MARGIN_SAV	62
#define MARGIN_MOD	64
#define MARGIN_CLR	36
#define MARGIN_XY	13
#define MARGIN_PEN	1
#define MARGIN_ANG	7

void	status_init ();
void	status_refresh ();

#endif