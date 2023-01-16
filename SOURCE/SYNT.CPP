#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <values.h>

#include "lexem.h"
#include "commands.h"
#include "synt.h"

CommandDesc cmdlist [] = {
	CMDSYS_NEW,	"NEW",		0, 0,	A_NUM, A_NUM, A_NUM, A_NUM,
	CMDSYS_RUN,	"RUN",		0, 0,	A_NUM, A_NUM, A_NUM, A_NUM,
	CMDSYS_SAVE,	"SAVE",		0, 0,	A_NUM, A_NUM, A_NUM, A_NUM,
	CMDSYS_LOAD,	"LOAD",		0, 0,	A_NUM, A_NUM, A_NUM, A_NUM,
	CMDSYS_DEL,	"DEL",		1, 2,	A_NUM, A_NUM, A_NUM, A_NUM,
	CMDSYS_EDIT,	"EDIT",		0, 1,	A_NUM, A_NUM, A_NUM, A_NUM,
	CMDSYS_CHANGE,  "CHANGE",	0, 1,	A_NUM, A_NUM, A_NUM, A_NUM,
	CMDSYS_INSERT,  "INSERT",	0, 1,	A_NUM, A_NUM, A_NUM, A_NUM,
	CMDSYS_LIST,	"LIST",		0, 2,	A_NUM, A_NUM, A_NUM, A_NUM,
	CMDSYS_QUIT,	"QUIT",		0, 0,	A_NUM, A_NUM, A_NUM, A_NUM,
	CMDSYS_LINES,	"LINES",	0, 0,	A_NUM, A_NUM, A_NUM, A_NUM,
	CMDSYS_PENHIDE, "PENHIDE",	0, 0,	A_NUM, A_NUM, A_NUM, A_NUM,
	CMDSYS_PENSHOW, "PENSHOW",	0, 0,	A_EXP, A_EXP, A_EXP, A_EXP,
	CMDSYS_PRINT,   "PRINT",	1, 1,   A_EXP, A_EXP, A_EXP, A_EXP,
	CMDSYS_BPTOGGLE,"BPTOGGLE",	1, 1,	A_EXP, A_EXP, A_EXP, A_EXP,
	CMDSYS_BPCLEAR,	"BPCLEAR",	0, 0,	A_EXP, A_EXP, A_EXP, A_EXP,
	CMD_CLS,	"CLS",		0, 0,	A_EXP, A_EXP, A_EXP, A_EXP,
	CMD_PENUP,	"PENUP",	0, 0,	A_EXP, A_EXP, A_EXP, A_EXP,
	CMD_PENDOWN,	"PENDOWN",	0, 0,	A_EXP, A_EXP, A_EXP, A_EXP,
	CMD_SETXY,	"SETXY",	2, 2,	A_EXP, A_EXP, A_EXP, A_EXP,
	CMD_SETX,	"SETX",		1, 1,	A_EXP, A_EXP, A_EXP, A_EXP,
	CMD_SETY,	"SETY",		1, 1,	A_EXP, A_EXP, A_EXP, A_EXP,
	CMD_PCOLOR,	"PCOLOR",	1, 1,	A_EXP, A_EXP, A_EXP, A_EXP,
	CMD_FWD,	"FWD",		1, 1,	A_EXP, A_EXP, A_EXP, A_EXP,
	CMD_BWD,	"BWD",		1, 1,	A_EXP, A_EXP, A_EXP, A_EXP,
	CMD_LEFT,	"LEFT",		1, 1,	A_EXP, A_EXP, A_EXP, A_EXP,
	CMD_RIGHT,	"RIGHT",	1, 1,	A_EXP, A_EXP, A_EXP, A_EXP,
	CMD_ANGLE,	"ANGLE",	1, 1,	A_EXP, A_EXP, A_EXP, A_EXP,
	CMD_REPEAT,	"REPEAT",	1, 1,	A_EXP, A_EXP, A_EXP, A_EXP,
	CMD_END,	"END",		0, 0,	A_EXP, A_EXP, A_EXP, A_EXP,
	CMD_PROC,	"PROC",		1, 1,	A_IDN, A_EXP, A_EXP, A_EXP,
	CMD_RETURN,	"RETURN",	0, 0,	A_EXP, A_EXP, A_EXP, A_EXP,
	CMD_CALL,	"CALL",		1, 1,	A_IDN, A_EXP, A_EXP, A_EXP,
	CMD_BCOLOR,	"BCOLOR",	1, 1,	A_EXP, A_EXP, A_EXP, A_EXP,
	CMD_LET,	"LET",		2, 2,	A_IDN, A_EXP, A_EXP, A_EXP,
	CMD_ADD,	"ADD",		2, 2,	A_IDN, A_EXP, A_EXP, A_EXP,
	CMD_SUB,	"SUB",		2, 2,	A_IDN, A_EXP, A_EXP, A_EXP,
	CMD_MUL,	"MUL",		2, 2,	A_IDN, A_EXP, A_EXP, A_EXP,
	CMD_DIV,	"DIV",		2, 2,	A_IDN, A_EXP, A_EXP, A_EXP,
	CMD_FILL,       "FILL",         1, 1,   A_EXP, A_EXP, A_EXP, A_EXP,
	CMD_ELLIPSE,	"ELLIPSE",	1, 4,	A_EXP, A_EXP, A_EXP, A_EXP,
	CMD_IF,		"IF",		1, 1,	A_EXP, A_EXP, A_EXP, A_EXP,
	CMD_ELSE,	"ELSE",		0, 0,	A_EXP, A_EXP, A_EXP, A_EXP,
	CMD_ENDIF,	"ENDIF",	0, 0,	A_EXP, A_EXP, A_EXP, A_EXP,
	CMD_WHILE,	"WHILE",	1, 1,	A_EXP, A_EXP, A_EXP, A_EXP,
	CMD_DELAY,	"DELAY",	0, 1,	A_EXP, A_EXP, A_EXP, A_EXP,
	CMD_LINESTYLE,	"LINESTYLE",	2, 2,	A_EXP, A_EXP, A_EXP, A_EXP,
	CMD_STOP,	"STOP",		0, 0,	A_EXP, A_EXP, A_EXP, A_EXP,
	0,		"",		0, 0,	A_NUM, A_NUM, A_NUM, A_NUM,
};

typedef struct stack {struct stack *next;int s;} stack;

static stack *ophead,*numhead;
int toppr;

int priority(char c)
{
 switch (c)
 {
  case '|' : return 1;
  case '&' : return 2;
  case '!' :
  case '=' : return 3;
  case '<' :
  case '>' : return 4;
  case '+' :
  case '-' : return 5;
  case '*' :
  case '/' :
  case '%' : return 6;
  case '^' : return 7;
 }
 return 0;
}


int pop(void)
{
 int c;
 stack *p;
 if (numhead)
 {
  c = numhead -> s;
  p = numhead -> next;
  delete numhead;
  numhead = p;
  return c;
 }
 else
  return 0;
}

int push(int c)
{
 stack *newtop = new stack;
 if (!newtop) return ERR_NOTMEM;
 newtop -> s = c;
 newtop -> next = numhead;
 numhead = newtop;
 return 0;
}

int oppop(void)
{
 int c;
 stack *p;
 if (ophead)
 {
  c = ophead -> s;
  p = ophead -> next;
  delete ophead;
  ophead = p;
  if (ophead)
    toppr = priority(ophead -> s);
  return c;
 }
 else
  return 0;
}

int oppush(int c)
{
 stack *newtop = new stack;
 if (!newtop) return ERR_NOTMEM;
 newtop -> s = c;
 newtop -> next = ophead;
 toppr=priority(c);
 ophead = newtop;
 return 0;
}

void syntax_init(void)
{
}

void swap(Lexem *cm1,Lexem *cm2)
{
 Lexem cmt;
 cmt=*cm1;
 *cm1=*cm2;
 *cm2=cmt;
}

int check_exp (Lexem * &cmnd, int *type)
{
 int sk = 0,prev;
 *type = 0;
 if (cmnd -> type == LEX_OPERAD || cmnd -> type == LEX_ZROVNO)
  return ERR_MISOPD;
 if (cmnd -> type == LEX_ENDSTR || cmnd -> type == LEX_PARMSEP)
  return ERR_MISPAR;
 while (cmnd -> type != LEX_ENDSTR && cmnd -> type != LEX_PARMSEP)
  switch (cmnd++ -> type)
  {
   case LEX_CMD:
   {
    *type = A_IDN;
    return ERR_RESRVD;
   }
   case LEX_NUM:
   {
    if (!*type) *type = A_NUM;
    if (cmnd -> type != LEX_OPERAD && cmnd -> type != LEX_ZAKRSK &&
	cmnd -> type != LEX_ENDSTR && cmnd -> type != LEX_PARMSEP)
	 return ERR_MISOPR;
    break;
   }
   case LEX_IDENT:
   {
    if (!*type) *type = A_IDN;
    if (cmnd -> type != LEX_OPERAD && cmnd -> type != LEX_ZAKRSK &&
	cmnd -> type != LEX_ENDSTR && cmnd -> type != LEX_PARMSEP)
	 return ERR_MISOPR;
    break;
   }
   case LEX_OPERAD:
   {
    *type = A_EXP;
    if (cmnd -> type != LEX_NUM && cmnd -> type != LEX_IDENT && cmnd -> type != LEX_OTKRSK)
     return ERR_MISOPD;
    break;
   }
   case LEX_OTKRSK:
   {
    if (cmnd -> type != LEX_OTKRSK && cmnd -> type != LEX_NUM && cmnd -> type != LEX_IDENT)
     return ERR_MISOPD;
    sk++;
    break;
   }
   case LEX_ZAKRSK:
   {
    if (cmnd -> type != LEX_ZAKRSK && cmnd -> type != LEX_OPERAD &&
	cmnd -> type != LEX_ENDSTR && cmnd -> type != LEX_PARMSEP)
	 return ERR_MISOPR;
    if (sk > 0)	sk--;
    else {
     cmnd--;
     return ERR_EXTAPR;
    }
    break;
   }
  }
 if (sk > 0) return ERR_MISAPR;
 return 0;
}

int check (Lexem *cmnd,int *errpos)
{
 int erc=0,param=0,i=2, tp;
 Lexem tmp = {LEX_PARMSEP,0,cmnd[2].pos_str}, *t;
 CommandDesc *ind = cmdlist;

 if (cmnd[0].type == LEX_IDENT && cmnd[1].type == LEX_ZROVNO)
 {
  cmnd[1].type=LEX_CMD;
  cmnd[1].value=CMD_LET;
  swap(&cmnd[0],&cmnd[1]);
  while (cmnd[i].type != LEX_ENDSTR) {swap(&tmp,&cmnd[i]);i++;}
  swap(&cmnd[i+1],&tmp);
  swap(&cmnd[i],&cmnd[i+1]);
 }
 *errpos = cmnd -> pos_str;
 if (cmnd -> type == LEX_CMD)
 {
  while (ind -> cmd != cmnd -> value) ind++;
  *errpos = (++cmnd) -> pos_str;
  while (cmnd -> type != LEX_ENDSTR)
  {
   if (param >= ind -> maxarg) return ERR_EXTPAR;
   erc = check_exp (cmnd, &tp);
   if (tp != ind -> argtypes [param] && ind -> argtypes [param] != A_EXP) return ERR_INVTYP;
   *errpos = cmnd -> pos_str;
   if (erc) return erc;
   param++;
   if (cmnd -> type == LEX_PARMSEP)
   {
    if (param >= ind -> maxarg)	return ERR_EXTSEP;
    *errpos = (++cmnd) -> pos_str;
    if (cmnd -> type == LEX_PARMSEP) return ERR_EXTSEP;
    if (cmnd -> type == LEX_ENDSTR) return ERR_MISPAR;
   }
   else
    if (param < ind -> minarg) return ERR_MISSEP;
  }
  if (param < ind -> minarg) return ERR_MISPAR;
 }
 else
  return ERR_NOTCMD;
 return 0;
}

int getcode (char *cname)
{
 int id=-1,i=0;
 while (cmdlist[i].cmd != 0)
 {
  if (!strcmpi(cmdlist[i].name,cname)) id = cmdlist[i].cmd;
  i++;
 }
 return id;
}

int calc()
{
 long a,b,r=0;

  a = pop();
  b = pop();
  switch (oppop())
  {
   case '+' : r=b+a;break;
   case '-' : r=b-a;break;
   case '*' : r=b*a;break;
   case '<' : r=b<a;break;
   case '>' : r=b>a;break;
   case '/' : if (a) r=b/a; else return ERR_DIVZER; break;
   case '%' : if (a) r=b%a; else return ERR_DIVZER; break;
   case '^' : r=a<32&&a||b?pow(b,a):-1; break;
   case mkword ('|','|') : r=b||a;break;
   case mkword ('&','&') : r=b&&a;break;
   case mkword ('=','=') : r=b==a;break;
   case mkword ('<','=') : r=b<=a;break;
   case mkword ('>','=') : r=b>=a;break;
   case mkword ('!','=') : r=b!=a;break;
  }
  if (r < -MAXINT || r > MAXINT)
   return ERR_OVRFLW;
  push(r);
  return 0;
}

int exec_ver(int *res,Lexem *cmnd,int *poz, int *poserr)
{
 int temp, erc = 0;
 ophead  = NULL;
 numhead = NULL;
 oppush(-1);
 while (cmnd[*poz].type != LEX_PARMSEP && cmnd[*poz].type != LEX_ENDSTR && !erc)
 {
  switch (cmnd[*poz].type)
  {
   case LEX_NUM :
   {
    erc = push(cmnd[*poz].value);
    if (erc) break;
    break;
   }
   case LEX_IDENT :
   {
    erc = get_nz_id(cmnd[*poz].value,&temp);
    if (erc) break;
    erc = push(temp);
    if (erc) break;
    break;
   }
   case LEX_OPERAD :
   {
     while(priority(cmnd[*poz].value) < toppr) {
       erc = calc();
       if (erc) break;
     }
    erc = oppush(cmnd[*poz].value);
    if (erc) break;
    break;
   }
   case LEX_OTKRSK :
   {
    erc = oppush(-1);
    if (erc) break;
    break;
   }
   case LEX_ZAKRSK :
   {
     while (ophead -> s != -1) {
	 erc = calc();
	 if (erc) break;
     }
     oppop();
    break;
   }
  }
  (*poz)++;
 }
 while (ophead -> s != -1) {
  erc = calc();
  if (erc) break;
 }
 while (ophead)
  oppop();
 while (numhead)
  *res = pop();
 *poserr = cmnd[*poz].pos_str;
 return erc;
}

char *getname(int code)
{
 CommandDesc *p=cmdlist;
 while (p -> cmd != code)p++;
 return (p -> name);
}

int getminpar(int code)
{
 CommandDesc *p=cmdlist;
 while (p -> cmd != code)p++;
 return (p -> minarg);
}