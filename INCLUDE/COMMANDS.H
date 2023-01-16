#ifndef __COMMANDS__
#define __COMMANDS__

#define CMD_MAX			256
#define CMD_MAXLEN		10
#define CMD_MAXARG		4

#define A_IDN			1	// Argument is identificator
#define A_NUM			2	// Argument is number
#define A_EXP			3	// Argument is expression

#define CMDSYS_NEW	    	1
#define CMDSYS_RUN		2
#define CMDSYS_SAVE		3
#define CMDSYS_LOAD		4
#define CMDSYS_DEL		5
#define CMDSYS_EDIT		6
#define CMDSYS_CHANGE		7
#define CMDSYS_INSERT		8
#define CMDSYS_LIST		9
#define CMDSYS_QUIT		10
#define CMDSYS_LINES		11
#define CMDSYS_PENHIDE		12
#define CMDSYS_PENSHOW		13
#define CMDSYS_PRINT		14
#define CMDSYS_BPTOGGLE		15
#define CMDSYS_BPCLEAR		16

#define CMDSYS_SAVEAS		50
#define CMDSYS_ABOUT		51
#define CMDSYS_CONTENTS		52
#define CMDSYS_INDEX		53
#define CMDSYS_TOPSEARCH	54
#define CMDSYS_PREVTOPIC	55

#define CMD_INTER		100

#define CMD_CLS			101
#define CMD_PENUP		102
#define CMD_PENDOWN		103
#define CMD_SETXY		104
#define CMD_SETX		105
#define CMD_SETY		106
#define CMD_PCOLOR		107
#define CMD_FWD			108
#define CMD_BWD			109
#define CMD_LEFT		110
#define CMD_RIGHT		111
#define CMD_ANGLE		112
#define CMD_REPEAT		113
#define CMD_END			114
#define CMD_PROC		115
#define CMD_RETURN		116
#define CMD_CALL		117
#define CMD_BCOLOR		118
#define CMD_LET			119
#define CMD_ADD			120
#define CMD_SUB			121
#define CMD_MUL			122
#define CMD_DIV			123
#define CMD_FILL		124
#define CMD_ELLIPSE		125
#define CMD_IF			126
#define CMD_ELSE		127
#define CMD_ENDIF		128
#define CMD_WHILE		129
#define CMD_DELAY		130
#define CMD_LINESTYLE		131
#define CMD_STOP		132

struct CommandDesc {
	int	cmd;
	char	name [CMD_MAXLEN];
	int	minarg, maxarg;
	char	argtypes [CMD_MAXARG];
};

#endif