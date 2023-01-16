#ifndef __BUFERMY__
#define __BUFERMY__

#include "lexem.h"
#define MAX_BUF 40
#define BUF_ERR_BADSTR 700
#define BUF_ERR_NOMEM 701
#define BUF_OK 0


int ins_cmd(Lexem *s, int n);
int chg_cmd(Lexem *s,int n);
int del_cmd(int n);
int Buf_Sum_Str(void);
Lexem *get_cmd(int n);
int buf_init(void);
int bp_reset ();
int toggle_bp_flag(int n);
int get_bp_flag(int n);

typedef struct List {
 struct List *next;
 struct List *prev;
 Lexem *spis;
 int bp_flag;
} List;

#endif //_BUFER_