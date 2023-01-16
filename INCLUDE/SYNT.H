#ifndef __SYNT__
#define __SYNT__

#include "lexem.h"

#define ERR_NOTCMD 201 //первая лексема - команда
#define ERR_INVTYP 202 //неверный тип параметра
#define ERR_MISSEP 203 //не хватает запятой
#define ERR_MISPAR 204 //не хватает параметров
#define ERR_EXTSEP 205 //лишняя запятая
#define ERR_EXTPAR 206 //лишний параметр
#define ERR_MISAPR 207 //отсутствует скобка
#define ERR_EXTAPR 209 //лишняя скобка
#define ERR_MISOPR 210 //отсутствует оператор
#define ERR_MISOPD 211 //отсутствует операнд
#define ERR_DIVZER 212 //деление на ноль
#define ERR_OVRFLW 213 //переполнение
#define ERR_NOTMEM 214 //не хватает памяти
#define ERR_RESRVD 215 //ключевое слово зарезервировано

void syntax_init(void); //Init. , done
int exec_ver (int *res,Lexem *cmnd,int *poss,int *poserr); //empty
int getcode (char *cname); //get code of command, done
int check (Lexem *cmnd,int *errpos);//checks command's syntax, done
char *getname(int code);
int getminpar(int code);

#endif
