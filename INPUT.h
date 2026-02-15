#ifndef INPUT_H
#define INPUT_H

#include <QtGui>

//---------------------------------------------------------------------------
extern int strfind(char *str1,char *str2);

//---------------------------------------------------------------------------
extern char check( char *number);
//---------------------------------------------------------------------------
extern void put_num(char *number, int index,QAbstractItemModel *model);
//---------------------------------------------------------------------------
extern void input(char in[20], int index, QAbstractItemModel *model);

#endif // INPUT_H
