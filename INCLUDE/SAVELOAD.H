#ifndef __FILEMY__                          //Комментарии:
#define __FILEMY__

#define SIGN "INTF"            		//Hаша сигнатура.
#define FILE_OPEN_W_ERR    300 		//Ошибка открытия файла для записи.
#define FILE_OPEN_R_ERR    301 	        //Ошибка открытия файла для чтение.
#define FILE_WRITE_ERR     302 		//Ошибка записи в файл.
#define FILE_READ_ERR      303          //Ошибка чтения файла.
#define FILE_FORMAT_ERR    304          //Ошибочный формат файла(не может быть интерпретирован).
#define GET_FROM_BUFER_ERR 305          //Ошибка получения данных из буфера.
#define PUT_IN_BUFER_ERR   306          //Ошибка помещения данных в буфер.
#define ID_ADDRESS_ERR     307          //Ошибочный адрес идентификатора.
#define ESC_PRESSED	   327

int select_file(char *s, char *hdr);
int save_file(char name_of_file[80]);   //Функция записи в файл(входной параметр-имя файла).
int load_file(char name_of_file[80]);   //Функция чтения файла(входной параметр-имя файла).
int ks();                               //Функция подсчета контрольной суммы(пока не работает).
void file_init();
void file_done();

#endif

