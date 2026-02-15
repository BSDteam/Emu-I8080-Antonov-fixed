#ifndef GLOBAL_H
#define GLOBAL_H

#include <QtGui>
#include "I8080.h"
#include "mainwindow.h"

/*-------------------------------------------------------*/

#define S_1 212
#define S_2 18
#define S_3 26

/*-------------------------------------------------------*/

extern QString GL_STR;
extern MainWindow *GL_MW;
extern  QStandardItemModel *GL_MODEL;
extern QTableView * GL_TV;

extern char comm[256][10];

extern int len_1[212];

extern int len_2[18];

extern int len_3[26];

extern const int len_3_count;
extern const int len_2_count;
extern const int len_1_count;

extern bool global_flag;


extern unsigned int PP,ON;
extern char st[5],fi[5];
extern char FIL[80];
extern char filename[80];
extern MICROPROC *processor;


#endif // GLOBAL_H
