#ifndef __XLOGO__
#define __XLOGO__

#define VERSION		"0.9 release"

#define SCREEN_X	640
#define SCREEN_Y	480

#define NAME_DEF	"NONAME.XLG"
#define HELP_FILENAME	"XLOGO.HLP"

#define VIEW_XSYM	60
#define VIEW_YSYM	38
#define VIEW_WDT	(VIEW_XSYM << 3)
#define VIEW_HGT	(VIEW_YSYM << 3)
#define VIEW_X1		((SCREEN_X - VIEW_WDT) / 2)
#define VIEW_Y1		40
#define VIEW_X2		(VIEW_X1 + VIEW_WDT)
#define VIEW_Y2		(VIEW_Y1 + VIEW_HGT)

#define CON_TXT_COLOR	LIGHTCYAN
#define CON_MSG_COLOR	YELLOW

#define CON_MSYM	100
#define CON_HGT         9
#define CON_VSYM	VIEW_XSYM
#define CON_X		VIEW_X1
#define CON_Y		(SCREEN_Y - 40 - (CON_HGT << 3))

#define ERR_INVSTR	100
#define ERR_CANCEL	101
#define ERR_BREAKPOINT	102

#define error_cont(err)								\
{										\
	int nerr = err;								\
	if (nerr && nerr != ERR_CANCEL) {					\
		error_con (nerr, cur_string, posstr);				\
		continue;							\
	}									\
}										\

extern "C" void logotype (void);

void get_sys_state (char *, int *, int *, int *, int *, int *);

#endif