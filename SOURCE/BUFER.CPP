#include <stdio.h>
#include <stdlib.h>
#include "bufer.h"
#include "lexem.h"

int SStr=0;
int tekstr=0;
/*
Lexem h[3] = {{1,1,1},
	      {2,2,2},
	      {5,0,5}};
*/

int LexSz(Lexem *buf)
{
 int t=0;

 while (buf[t++].type!=LEX_ENDSTR);
 return t;
}


List *head = NULL,*nach = NULL, *tek = NULL;


int ins_cmd(Lexem *s, int n)
{
 int kol=1,Col_Lex=0;
 List *p,*c;
 Lexem *ptr;

 p = (List *) malloc(sizeof(List));
 if (p==NULL) return BUF_ERR_NOMEM;

 if (n>(SStr+1) || n < 0) {
  return BUF_ERR_BADSTR;
 };
 if (head==NULL) {
  p -> next = NULL;
  p -> prev = NULL;
  SStr++;
  head = p;
  nach = p;
 } else if (n==1) {
  p -> next = nach;
  SStr++;
  nach -> prev = p;
  p -> prev = NULL;
  nach = p;
 } else if (n==SStr+1) {
  head -> next = p;
  p -> prev = head;
  SStr++;
  p -> next = NULL;
  head = p;
 } else {
  c = nach;
  while (n!=kol) {
   c = c -> next;
   kol++;
  }
  p -> prev = c -> prev;
  c -> prev -> next = p;     // !!!!!!!!!
  c -> prev = p;
  p -> next = c;
  SStr++;
 }

 Col_Lex = LexSz(s);
 ptr = (Lexem *) malloc(sizeof(Lexem)*Col_Lex);
 if (ptr == NULL) return BUF_ERR_NOMEM;
 int t=0;
 for (t=0;t<Col_Lex;t++) {
  ptr[t].type = s[t].type;
  ptr[t].value = s[t].value;
  ptr[t].pos_str = s[t].pos_str;
 };
 p -> spis = ptr;
 p -> bp_flag = 0;

 // copy s -> ptr
 return BUF_OK;
}

int chg_cmd(Lexem *s,int n)
{
 List *c;
 int kol = 1,Col_Lex=0;
 Lexem *ptr;

 c = nach;
 if (SStr <= 0) {
  return BUF_ERR_BADSTR;
 } else if ((n>SStr) || (n<0)) {
  return BUF_ERR_BADSTR;
 };
 while (kol!=n)  {
  c = c -> next;
  kol++;
 }

 free(c -> spis);

 Col_Lex = LexSz(s);
 ptr = (Lexem *) malloc(sizeof(Lexem)*Col_Lex);
 if (ptr == NULL) return BUF_ERR_NOMEM;
 int t=0;
 for (t=0;t<Col_Lex;t++) {
  ptr[t].type = s[t].type;
  ptr[t].value = s[t].value;
  ptr[t].pos_str = s[t].pos_str;
 };
 c -> spis = ptr;

 return 0;
}

int del_cmd(int n)
{
 List *c,*k;

 if (SStr == 0) {
  return BUF_ERR_BADSTR;
 } else if (n>SStr || n<0) {
  return BUF_ERR_BADSTR;
 };
 if (n==SStr && n==1) {
  k = head;
  head = NULL;
  nach = NULL;
  SStr--;
 } else				//!!!!!
 if (n==1) {
  k = nach;
  nach = nach->next;
  SStr--;
  nach->prev = NULL;
 } else if (n==SStr) {
  k = head;
  head = head -> prev;
  head -> next = NULL;
  SStr--;
 } else {
  int kol = 1;

  c = nach;
  while (kol!=n) {
   c = c -> next;
   kol++;
  }
  k = c;
  c = c -> prev;
  c -> next = c -> next -> next;
  c = c -> next;
  SStr--;
  c -> prev = c -> prev -> prev;
 }

 free(k->spis);
 free(k);

 return 0;
}

int Buf_Sum_Str()
{
 return SStr;
}

int bp_reset ()
{
 List *c = nach;
 while (c) {
   c -> bp_flag = 0;
   c = c -> next;
 }
 return 0;
}

int toggle_bp_flag(int n)
{
 List *c;
 int kol = 1;
 if (SStr==0) {
  return BUF_ERR_BADSTR;
 } else if ((n>SStr) || (n<=0)) {
  return BUF_ERR_BADSTR;
 } else {
  c = nach;
  if (kol!=n) { while (kol!=n) {
   c = c -> next;
   kol++;
  }};
 }
 c -> bp_flag = !c -> bp_flag;
 return 0;
}

int get_bp_flag(int n)
{
 List *c;
 int kol = 1;
 if (SStr==0) {
  return 0;
 } else if ((n>SStr) || (n<=0)) {
  return 0;
 } else {
  c = nach;
  if (kol!=n) { while (kol!=n) {
   c = c -> next;
   kol++;
  }};
 }
 return c -> bp_flag;
}

Lexem *get_cmd(int n)
{       // return &program[i];
 List *c;
 int kol = 1;

 if (SStr==0) {
  return NULL;
 } else if ((n>SStr) || (n<=0)) {
  return NULL;
 } else {
  c = nach;
  if (kol!=n) { while (kol!=n) {
   c = c -> next;
   kol++;
  }};
 }
 return c -> spis;
// return NULL;
}

int buf_init(void)
{
 while (SStr>0) {
  del_cmd(1);
 }
/* if (SStr!=0) {
  for (i=1;i<=SStr;i++)
   del_cmd(1);
 }*/
 SStr=0;
 return 0;
}
/*
int main(void)
{
 SStr=0;
 ins_cmd(&h[0],1);
 ins_cmd(&h[1],1);
 ins_cmd(&h[2],1);
 buf_init();
 return 0;
} */