#include <stdio.h>
#include <conio.h>
#include <io.h>
#include <string.h>
#include <stdlib.h>
#include <dir.h>
#include <dos.h>

#include "bufer.h"
#include "graph.h"
#include "lexem.h"
#include "saveload.h"
#include "listbox.h"
#include "xlogo.h"
#include "edit.h"
#include "dialogs.h"
#include "keyboard.h"

#define TEST_WRITE(item,size_type,count) \
      if(fwrite(item,size_type,count,f)!=count){ \
	   fclose(f); \
	   return (FILE_WRITE_ERR); \
	 }
#define TEST_READ(item,size_type,count)\
      if((fread(item,size_type,count,f))!=count) {\
       fclose(f);\
       return(FILE_READ_ERR);\
      }
#define XH_LIST 248
#define YH_LIST 200
#define XD_LIST 392
#define YD_LIST 350
#define WT_EDIT 16
#define WT_LIST 16
#define HT_LIST 15
#define DIR_COLOR 15
#define FILE_COLOR 11

 char old_path[80], mask[15] = "*.XLG";
 unsigned int cons=0;
 int edit_ret, list_ret, n_file;
 EditSettings es;

 struct FileInfo {
	char name [13];
	char attrib;
	int  time;
	int  date;
	long size;
 };

FileInfo pardir = {"..", FA_DIREC, 0, 0, 0};

FileInfo *infoconv (FileInfo *fi1, ffblk *fi2)
{
	strcpy (fi1 -> name, fi2 -> ff_name);
	fi1 -> attrib	= fi2 -> ff_attrib;
	fi1 -> time	= fi2 -> ff_ftime;
	fi1 -> date	= fi2 -> ff_fdate;
	fi1 -> size	= fi2 -> ff_fsize;
	return fi1;
}

void listfunc (int *key)
{
	if (*key == K_TAB) {
		list_ret = 1;
		*key = K_ENTER;
	}
}

int sortfunc (void *fi1, void *fi2)
{
	FileInfo *f1 = (FileInfo *)fi1, *f2 = (FileInfo *)fi2;
	if ((f1 -> attrib & FA_DIREC) < (f2 -> attrib & FA_DIREC))
		return 1;
	return strcmp (f1 -> name, f2 -> name);
}

void see_files()
  {
    int finder;
    ffblk file_info;
    char dir[MAXDIR];
    FileInfo fi;

    listbox_init(XH_LIST+8, YH_LIST+24,WT_LIST,HT_LIST,listfunc, sortfunc);
    finder=findfirst("*.*",&file_info,FA_DIREC);

    getcurdir (0, dir);
    if (*dir) listbox_add (&pardir, sizeof pardir, DIR_COLOR);
    while(!finder) {
	   if (file_info.ff_attrib&FA_DIREC)
	      if (*file_info.ff_name!='.')
		 listbox_add (infoconv (&fi, &file_info), sizeof fi, DIR_COLOR);
	   finder=findnext(&file_info);
    }
    finder=findfirst(mask,&file_info,0);
    while(!finder) {
	   listbox_add (infoconv (&fi, &file_info), sizeof fi, FILE_COLOR);
	   finder=findnext(&file_info);
    }
  }

int error_pathnotfound ()
{
	dialog_init (256, 240, 384, 308, "Ошибка");
	dialog_statictext (8, 10, WHITE, "Путь не найден");
	dialog_button (DLG_OK, 32, 40, 8, "ОК");
	dialog_run ();
	dialog_done ();
	return 0;
}

void editfunc (int *key, int *curpos, char *str)
{
	int		err, fns_res;
	char		drive[MAXDRIVE], dir[MAXDIR], file[MAXFILE], ext[MAXEXT];
	FileInfo	fl;

	switch (*key) {
		case K_ENTER:
		case K_TAB:
			fns_res = fnsplit (str, drive, dir, file, ext);
			if (!(fns_res & EXTENSION))
				strcpy (ext, ".XLG");
			if (!(fns_res & FILENAME))
				strcpy (file, "*");
			if (fns_res & WILDCARDS)
				sprintf (mask, "%s%s", file, ext);
			else if (fns_res & FILENAME) {
				edit_ret = 0;
				break;
			}
			if (fns_res & DRIVE)
				if (setdisk (*drive - 'A') <= *drive - 'A') {
					error_pathnotfound ();
					break;
				}
			if (fns_res & DIRECTORY)
				if (chdir (dir)) {
					error_pathnotfound ();
					break;
				}
			list_ret = 0;
			do {
				fnmerge (str, drive, dir, file, ext);
				*curpos = strlen (str);
				edit_draw (&es, str, 0);
				see_files ();
				n_file	= 0;
				err	= listbox_run (&n_file, &fl);
				listbox_done ();
				if (err) {
					list_ret = edit_ret = 1;
					*key = K_ENTER;
					break;
				}
				if (!list_ret && fl.attrib & FA_DIREC)
					if (chdir (fl.name))
						error_pathnotfound ();
					else {
						strcpy (drive, "X:");
						*drive	= 'A' + getdisk ();
						*dir	= '\\';
						getcurdir (0, dir + 1);
					}
			} while (fl.attrib & FA_DIREC && !list_ret);
			if (!list_ret) {
				strcpy (str, fl.name);
				edit_ret = 0;
				*key	 = K_ENTER;
			}
			break;
		case K_ESCAPE:
			edit_ret = 1;
			*key = K_ENTER;
			break;
	}
}


int select_file(char *s, char *hdr)
{
	int	cp, fns_res;
	char	drive[MAXDRIVE], dir[MAXDIR], file[MAXFILE], ext[MAXEXT];

	openhwindow (XH_LIST, YH_LIST, XD_LIST, YD_LIST, hdr);
	n_file		= 0;
	edit_ret	= -1;
	strcpy (s, mask);
	cp = strlen (s);
	see_files ();
	listbox_draw (0);
	listbox_done ();
	while (edit_ret < 0) edit (&es, s, &cp);
	closewindow ();
	if (edit_ret)
		return ERR_CANCEL;
	return 0;
}

 //19.11.99 -Глюк в том что ф-ция сифайл не может отличить файл от директории!!!
 //файл выбирается но в него ничего не пишется из-за несоответствия счетчиков
 //i и count_dir(count_dir равен count_file)(См немного выше/*!!!*/)

int ks(char *c,int n)
{ int i;
  for(i=0;i<n;i++) cons=cons+c[i];
 return 0;
}

 int save_file(char name_of_file[80])
  {
     char c,*str;
     Lexem *lex;
     char *id;
     FILE *f;
     int s,i,num,cs;
     if ((f=fopen(name_of_file,"wb"))==NULL){
	   //fclose(f);
	   return (FILE_OPEN_W_ERR);
	 }
	cons=0;
      ks(SIGN,4);
      TEST_WRITE(SIGN,4,1);
      num=Buf_Sum_Str();
      ks((char *)&num,2);
      TEST_WRITE(&num,sizeof(int),1);
      for(s=1;s<=num;s++)
	 {
	  lex=get_cmd(s);
	  if (lex==NULL) break;
	      i=0;
	      while(lex[i++].type!=LEX_ENDSTR);
	  ks((char *)&i,2);
	  ks((char *)lex,sizeof(Lexem)*i);
	  TEST_WRITE(&i,sizeof(int),1);
	  TEST_WRITE(lex,sizeof(Lexem),i);
	 }
     num=id_sum();
     ks((char *)&num,sizeof(int));
     TEST_WRITE(&num,sizeof(int),1);
      for(s=0;s<num;s++)
	 {
	  id=get_id(s);
	  i=strlen(id)+1;
	  ks((char *)&i,sizeof(int));
	  ks(id,sizeof(char)*i);
	  TEST_WRITE(&i,sizeof(int),1);
	  TEST_WRITE(id,i,1);
	 }
	 TEST_WRITE(&cons,sizeof(int),1);
	 fclose(f);

/*
signatura
size prog
programma
size id
identif
ks
*/
return 0;
     }

 int control_sum_test(char name_of_file[80])
     {
      char sig[5],id[20];
      Lexem lex[20];
      FILE *f;
      int s,i,k,num,o,control=0,num2;
      cons=0;
      if ((f=fopen(name_of_file,"rb"))==NULL){
       //fclose(f);
       return (FILE_OPEN_R_ERR);
      }
      TEST_READ(sig,4,1);
	 sig[4]=0;
       ks(sig,4);
      if (!strcmp(sig,SIGN)){
      TEST_READ(&num,sizeof(int),1);
      ks((char*)&num,sizeof(int));
	  for (i=1; i<=num; i++)
	   {
	    TEST_READ(&k,sizeof(int),1);
	    ks((char*)&k,sizeof(int));
	    TEST_READ(lex,sizeof(Lexem),k);
	    ks((char*)lex,sizeof(Lexem)*k);
	   }
	    TEST_READ(&num2,sizeof(int),1);
	    ks((char*)&num2,sizeof(int));
	   for(s=0;s<num2;s++)
		 {
		  TEST_READ(&i,sizeof(int),1);
		  ks((char*)&i,sizeof(int));
		  TEST_READ(id,sizeof(char),i);
		  ks(id,sizeof(char)*i);
		 }
		TEST_READ(&control,sizeof(int),1);
		if (cons!=control) {
		    fclose(f);
		    return(FILE_FORMAT_ERR);
		  }

	 fclose(f);
	 return 0;
	 }
	 else
	 {
	  fclose(f);
	  return(FILE_FORMAT_ERR);

	 }
     }

 int load_file(char name_of_file[80])
     {
      char sig[5],id[20];
      Lexem lex[20];
      FILE *f;
      int s,i,k,num,o,control;
      control=control_sum_test(name_of_file);
      if (control!=0) return(control);
      buf_init();
      lex_init();
      if ((f=fopen(name_of_file,"rb"))==NULL){
       // fclose(f);
       return (FILE_OPEN_R_ERR);
      }
      TEST_READ(sig,4,1);
	 sig[4]=0;
       ks(sig,4);
      if (!strcmp(sig,SIGN))
      {
      TEST_READ(&num,sizeof(int),1);
	  for (i=1; i<=num; i++)
	   {
	    TEST_READ(&k,sizeof(int),1);
	    TEST_READ(lex,sizeof(Lexem),k);
	    if((ins_cmd(lex,i))!=0) {
	     fclose(f);
	     return(PUT_IN_BUFER_ERR);
	    }
	   }
	    TEST_READ(&num,sizeof(int),1);
	   for(s=0;s<num;s++)
		 {
		  TEST_READ(&i,sizeof(int),1);
		  TEST_READ(id,sizeof(char),i);
		  if ((put_id(id,&i))!=0) {
		   fclose(f);
		   return (PUT_IN_BUFER_ERR);
		   }//если ошибка помещения идентификатора в буфер }
		  if (i != s) {
		    fclose(f);
		    return (ID_ADDRESS_ERR);
		   }//если полученный адрес ид-ра неверный
		 }
		TEST_READ(&control,sizeof(int),1);
	 fclose(f);
	 return 0;
	 }
	 else
	 {
	  fclose(f);
	  return(FILE_FORMAT_ERR);
	 }
     }

     //ф-ция Си-файл возвращает массив имен файлов в текущей директории
     //разобраться со значением attr  в этой функции-по attr ищутся
     //либо файлы либо директории
     //сделать сортировку массива имен файлов перед его возвращением
     //посмотреть что там с change dir
   void file_init()
       {
	 strcpy(old_path, "X:\\");
	 old_path[0] = 'A' + getdisk();
	 getcurdir (0, old_path+3);
	 edit_init (&es, XH_LIST+8, YH_LIST+8, WT_EDIT, 80, BLUE, WHITE, editfunc);
       }
   void file_done()
       {
	 setdisk (*old_path - 'A');
	 chdir(old_path);
       }