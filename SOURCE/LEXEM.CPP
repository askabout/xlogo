#include <string.h>
#include <math.h>
#include <stdio.h>
#include <ctype.h>
#include <alloc.h>
#include <stdlib.h>
#include "lexem.h"
#include "bufer.h"
#include "synt.h"

static Ident *head=NULL;
TZnach *znach=NULL;

int summlex=0,numlex=0,peremen=0;

int ins_id(char *x,Ident * &p, int *id)
{
 int i=0;

 if (!p) {
  *id = peremen;
  if (!(p = new Ident)) return NOT_EN_MEM;
  p->znach = peremen;
  p->name = strdup (x);
  if (!(peremen % IDENT_REALLOC))
    if (!(znach = (TZnach *)realloc (znach, sizeof (TZnach) * (peremen+IDENT_REALLOC))))
      return NOT_EN_MEM;
  znach[peremen].znach=0;
  znach[peremen].is=0;
  znach[peremen].init=0;
  znach[peremen++].name=p->name;
  p->left = NULL;
  p->right = NULL;
  return 0;
 }
  i= strcmp(x,p->name);
  if (!i) { *id = p->znach; return 0; }
  if (i<0)
    return ins_id(x,p->left,id);
  if (i>0)
    return ins_id(x,p->right,id);
 return 0;
}

int iszna(char *s,int *i,int *z)
{
 *z = s[*i];
 switch (s[(*i)++]) {
  case '+':
  case '-':
  case '*':
  case '/':
  case '%':
  case '^': return LEX_OPERAD;
  case '<':
  case '>':
  case '=':
  case '!':
    if (s[*i] == '=') {
      *z = mkword (*z, '=');
      (*i)++;
      return LEX_OPERAD;
    }
    if (*z == '=') return LEX_ZROVNO;
    else return LEX_OPERAD;
  case '|':
  case '&':
    if (s[*i] == *z) {
      *z = mkword (*z, *z);
      (*i)++;
      return LEX_OPERAD;
    }
    break;
  case '(': return LEX_OTKRSK;
  case ')': return LEX_ZAKRSK;
 }
 return 0;
}

int mkint(char *s,int *i, int *ind)
{
 long for_int=0, z = 0;

 while (isdigit (s[*i])) {
  for_int=for_int*10+s[*i]-'0';
  (*i)++;
  z++;
  if (z > 5) return NOT_RIGHT_VALUE;
 }
 if (for_int > MAX_INT_VALUE) return NOT_RIGHT_VALUE;
 *ind = (int)for_int;
 return 0;
}

int put_id(char *s,int *d)
{
 return ins_id(s,head,d);
}

int mklex(int type,int value,int pos,Lexem *lex)
{
 if (summlex==MAX_NUM_LEXEM) return TOO_MANY_LEXEM;
 lex[summlex].type = type;
 lex[summlex].value = value;
 lex[summlex++].pos_str = pos;
 return 0;
}

int mkaid(char *s,int *i,int *id,int *type)
{
 int h=*i;
 int save;
 int err = 0;

 while (isalpha(s[*i]) || (isdigit(s[*i]))) (*i)++;
 save = s[*i];
 s[*i]=0;
 if ((*id = getcode(&s[h]))!=-1) {
  *type = LEX_CMD;
 } else {
  *type = LEX_IDENT;
  err = put_id(&s[h],id);
 }
 s[*i] = save;
 return err;
}

int rasbiv(char *s,Lexem *lex,int *i)
{
 int k,t,err,type;
 summlex=0;

 *i = 0;
 while ( s[*i] ) {
   while (s[*i]==' '&&s[*i]) (*i)++;
   if (!s[*i]) break;
   k = *i;
   if (s[k] == ',') {
	if ((err = mklex(LEX_PARMSEP,err,k,lex))!=0) return err;
	k++;
   }
   else if (isalpha(s[k])) {
	if ((err = mkaid(s,&k,&t,&type))!=0) return err;
	if ((err = mklex(type,t,*i,lex))!=0) return err;
   }
   else if (isdigit(s[k])) {
	if ((err = mkint(s,&k,&t))!=0) return err;
	if ((err = mklex(LEX_NUM,t,*i,lex))!=0) return err;
   }
   else if ((err=iszna(s,&k,&t))!=0)
	  if (err==LEX_ZROVNO) mklex(LEX_ZROVNO,0,*i,lex);
	  else mklex(err,t,*i,lex);
   else return BAD_SIM;
   *i = k;
 }
 if ((err = mklex(LEX_ENDSTR,0,k,lex))!=0) return err;
 return 0;
}

void del(Ident * &q, Ident * &r)
{
 if (r -> right)
  del (q, r -> right);
 else {
  q -> znach = r -> znach;
  q -> name = r -> name;
  q = r;
  r = r -> left;
 }
}

int del_id (char *name, Ident * &p)
{
 int x;
 Ident *q;

 if (!p) return 0;
 x = strcmp (name, p -> name);
 if (x < 0) return del_id (name, p->left);
 if (x > 0) return del_id (name, p->right);
 q = p;
 if (!q->right) p = q->left;
 else if (!q->left) p = q->right;
 else del (q, q->left);
 delete q;
 return 0;
}

void id_trans(Ident * &ptr)
{
 if (ptr) {
  id_trans(ptr->left);
  ptr->znach = znach [ptr->znach].trans;
  id_trans(ptr->right);
 }
}

int id_optimize ()
{
 Lexem *l;
 int i, j, k;

 for (i = 1; i <= Buf_Sum_Str (); i++) {
  l = get_cmd (i);
  while (l -> type != LEX_ENDSTR) {
   if (l -> type == LEX_IDENT)
    znach [l -> value].is = 1;
   l++;
  }
 }
 i = 0;
 k = 0;
 while (i < peremen) {
  if (!znach [i].is) {
   del_id (znach [i].name, head);
   free (znach [i].name);
   peremen--;
   k++;
   for (j = i; j < peremen; j++) {
    znach[j].is = znach[j+1].is;
    znach[j].init = znach[j+1].init;
    znach[j].znach = znach[j+1].znach;
    znach[j].name = znach[j+1].name;
    znach[j+k].trans = j;
   }
  } else {
   znach [i].trans = i;
   znach [i++].is  = 0;
  }
 }
 for (i = 1; i <= Buf_Sum_Str (); i++) {
  l = get_cmd (i);
  while (l -> type != LEX_ENDSTR) {
   if (l -> type == LEX_IDENT)
    l -> value = znach [l -> value].trans;
   l++;
  }
 }
 id_trans (head);
 return 0;
}

int id_sum(void)
{
 return peremen;
};

int put_nz_id(int n,int zna)
{
 if (n<=peremen) {
  znach[n].znach=zna;
  znach[n].init=1;
  return 0;
 }
 return NOT_RIGHT_NUMBER_ID;
}

int get_nz_id(int n,int *zna)
{
 if (n<=peremen) {
  *zna=znach[n].znach;
  return znach[n].init ? 0 : NOT_DEFINED;
 }
 return NOT_RIGHT_NUMBER_ID;
}

char *get_id(int n)
{
 if (n<peremen) {
  return znach[n].name;
 }
 return NULL;
}

int ochist_id(Ident * &ptr)
{
 if (ptr) {
  ochist_id(ptr->left);
  free (ptr -> name);
  delete ptr;
  ochist_id(ptr->right);
 }
 return 0;
}

int lex_done()
{
 ochist_id(head);
 if (znach) free (znach);
 peremen=0;
 head=NULL;
 znach=NULL;
 return 0;
}

int lex_init()
{
 if (head) lex_done();
 return 0;
}

void ident_reset ()
{
 for (int i=0; i<peremen;i++)
  znach[i].init=0;
}
