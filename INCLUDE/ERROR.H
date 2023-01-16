#ifndef __ERROR__
#define __ERROR__

#include <fstream.h>
#include <process.h>

#define error_fatal(err)						\
{									\
	asm mov ax, 03h;						\
	asm int 10h;							\
	cerr << __FILE__ << "(" << __LINE__ << ") : " << err << endl;	\
	exit (1);							\
}

#endif