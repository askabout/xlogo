#ifndef __INTER__
#define __INTER__

#define INT_ERR_BEDNUM			1
//{ INT_ERR_BEDNUM, 		"Нет строки с таким номером"}
#define INT_ERR_OUT_OF_SCREEN		2
//{INT_ERR_OUT_OF_SCREEN, 	"Выход за пределы экрана рисования"}
#define INT_ERR_INVALID_COLOR   	3
//{INT_ERR_INVALID_COLOR, 	"Неизвестный номер цвета"}
#define INT_ERR_OUT_OF_REAL_SCREEN	4
//{INT_ERR_OUT_OF_REAL_SCREEN,	"Ошибка нициализации окна для интерпритатора"}
#define INT_ERR_SECOND_PROC		5
//{INT_ERR_SECOND_PROC,		"Повторное описание процедуры"}
#define INT_ERR_RETURN_PROC		6
//{INT_ERR_RETURN_PROC,		"Ошибка оператора Return нет заголовка процедуры"}
#define INT_ERR_UNYKNOW_PROC		7
//{INT_ERR_UNYKNOW_PROC,	"Процедура должна быть описана ранее"}
#define INT_ERR_RETURN_REPEAT		8
//{INT_ERR_RETURN_REPEAT,	"Ошибка оператора End нет заголовка цикла"}
#define INT_ERR_OVERFLOW		9
//{INT_ERR_OVERFLOW,		"Недопустимая операция, переполнение"}
#define INT_ERR_DIVISION_BY_ZERO	10
//{INT_ERR_DIVISION_BY_ZERO,	"Недопустимая операция, деление на ноль"}
#define INT_ERR_SECOND_PROC_IN		11
//{INT_ERR_SECOND_PROC_IN,	"Недопустимое вложеное описание процедуры"}
#define ERR_INT_PROC_RECURCE		12
//{ERR_INT_PROC_RECURCE,	"Ошибка. Недопустима рекурсия."}
#define INT_ERR_UNDERFLOW		13
//{INT_ERR_UNDERFLOW,		"Отрицательное число"}
#define INT_ERR_USER_BREAK		14
//{INT_ERR_USER_BREAK		"Выполнение программы прервано пользователем"}
#define INT_ERR_MISSING_END		15
//{INT_ERR_MISSING_END,		"Пропущен оператор END при описании процедуры"}
#define INT_ERR_MISSING_IF		16
//{INT_ERR_MISSING_IF,		"Пропущен оператор IF при описании условия"}
#define INT_ERR_IF_IN_PROC		17
//{INT_ERR_IF_IN_PROC,		"Ошибка при описании условия IF"}
#define INT_ERR_MISSING_RETURN		18
//{INT_ERR_MISSING_RETURN,	"Пропущен оператор RETURN при описании процедуры"}
#define INT_ERR_UNIKNOWN_STYLE		19
//{INT_ERR_UNIKNOWN_STYLE,	"Неизвестный тип линии"}
#define INT_ERR_REPEAT_MISSING_ENDIF	20
//{INT_ERR_REPEAT_MISSING_ENDIF	"Пропущен оператор ENDIF, при описании цикла"}
void draw_pen ();
int get_max_x ();
int get_max_y ();
int inter_init(int x1,int y1,int x2,int y2);
int inter(int *numstr,int *poserr);
void get_inter_stat(int *color,int *move,int * x, int *y, int *ang);
int inter_reset(void);
#endif
