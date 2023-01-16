#ifndef __LEXEM__
#define __LEXEM__

#define MAX_NUM_LEXEM           30
#define MAX_INT_VALUE           32767
#define LEX_CMD			1	// Команда
#define LEX_NUM			2	// Целое число
#define LEX_IDENT		3	// Идентификатор
#define LEX_PARMSEP		4	// Разделитель пар-ов (,)
#define LEX_ENDSTR		5	// End of string
#define LEX_OPERAD		6
#define LEX_OTKRSK		7
#define LEX_ZAKRSK		8
#define LEX_ZROVNO		9
#define TOO_MANY_LEXEM          500
#define NOT_RIGHT_NUMBER	502
#define NOT_RIGHT_VALUE		503
#define NO_VALUE		504
#define BAD_SIM			505
#define NOT_RIGHT_NAME_ID	506
#define NOT_RIGHT_NUMBER_ID	507
#define NOT_EN_MEM		508
#define NOT_DEFINED		509
#define IDENT_REALLOC		5

#define mkword(c1,c2)		((c2<<8)+c1)

struct TZnach{
	char *name;
	int znach, trans;
	char is, init;
};

struct Ident{
	char *name;
	Ident *left;
	Ident *right;
	int znach;
};

typedef struct {
	int type, value, pos_str;
} Lexem;

int id_sum(void);
char *get_id(int n);
int rasbiv(char *s,Lexem *lex,int *i);
int put_id(char *s,int *d);
int put_nz_id(int n,int zna);
int get_nz_id(int n,int *zna);
int id_optimize();
int lex_done();
int lex_init();
void ident_reset();
#endif // __LEXEM__