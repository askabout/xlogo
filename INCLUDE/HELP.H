#ifndef __HELP__
#define __HELP__

#define MAX_TOPICS	100
#define MAX_TOPBUFFER	50
#define MAX_REFS	100
#define MAX_TOPLEN	20

#define HELP_MAXHGT	70
#define HELP_WDT	40
#define HELP_HGT	25
#define HELP_X		160
#define HELP_Y		100

#define HELP_TXTCOLOR	WHITE
#define HELP_CURCOLOR	LIGHTCYAN
#define HELP_REFBGCOLOR	BLUE
#define HELP_REFFGCOLOR	YELLOW

#define ERR_HELPOPEN		400
#define ERR_TOOMANYTOP		401
#define ERR_TOPICNOTFOUND	402
#define ERR_WORDTOOBIG		403
#define ERR_TEXTTOOBIG		404
#define ERR_TOOMANYREF		405

char *errors(int kod);
int help_init (char *f);
int help_prevtopic ();
int help (char *topic);
int help_done ();

#endif