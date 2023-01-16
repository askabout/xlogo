#ifndef __KEYBOARD__
#define __KEYBOARD__

#define K_ESCAPE	0x011B
#define K_BACKSPACE	0x0E08
#define K_TAB		0x0F09
#define K_STAB		0x0F00
#define K_ENTER		0x1C0D
#define K_SPACE		0x3920
#define K_HOME		0x4700
#define K_UPARROW	0x4800
#define K_LEFTARROW	0x4B00
#define K_RIGHTARROW	0x4D00
#define K_DOWNARROW	0x5000
#define K_CLEFTARROW	0x7300
#define K_CRIGHTARROW	0x7400
#define K_END		0x4F00
#define K_PAGEDOWN	0x5100
#define K_PAGEUP	0x4900
#define K_INS		0x5200
#define K_DEL		0x5300
#define K_F(x)		(0x3A + x << 8)
#define K_SF(x)		(0x53 + x << 8)
#define K_CF(x)		(0x5D + x << 8)
#define K_AF(x)		(0x67 + x << 8)
#define K_ALTX		0x2D00
#define K_CTRLC		0x2E03
#define K_CTRLBREAK	0x0000

#endif
