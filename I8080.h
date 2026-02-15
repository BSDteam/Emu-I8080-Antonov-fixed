#ifndef I8080_H
#define I8080_H

#include <stdio.h>
#include <QtGui>
#include <QTableView>
#include <stdlib.h>
#include <time.h>
#include "mem.h"

/*description of class MICROPROC including alu function*/

const unsigned int  MEMSIZE=65535;
const unsigned int Default_SP=0xf000;
const unsigned int Default_IP=0x1000;


struct RGFLAG
{
    unsigned char C,P,Z,S;
};


namespace Ui {
class MainWindow;
}

class MainWindow;

class MICROPROC
{  //friend class IOI;//IOI - input/output interface
  public:
   unsigned char A,B,C,D,E,H,L;//proc rg
   RGFLAG flag;
   unsigned int IP;
   unsigned int SP;
   unsigned char RgTimer;
   MainWindow *Mw;
   time_t t;

   //unsigned char *memory;
   mem memory;
   int alu(unsigned char code);
   MICROPROC(QTableView *mainTable,QTextBrowser *out10,MainWindow *mw);
   ~MICROPROC();

    QString buf;

   static QString put_hex4(unsigned int dig);
   static QString put_hex2(unsigned int dig);
   QTableView *MainTable;
   QTextBrowser *Out10;
private:




   void set_flags(int number);
};

#endif // I8080_H
