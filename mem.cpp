#include "mem.h"

mem::mem()
{
    isActiv= new bool;
    *isActiv=false;
    this->data=new unsigned char [256*256];
    this->command=new unsigned char [256*256];
    for (int i=0; i<256*256; i++)this->command=0;
}

unsigned char& mem::operator[]( int index)
{
   // qDebug()<<index<<data[index];
    return this->data[index];
}


void mem::SetTable (QTableView *t)
{
    table=t;
}

void mem::SetFlag(bool &fl)
{
    *isActiv=fl;
}

QString mem::put_hex2(unsigned int dig)
{
    QString result,buf;
    if (dig>=0x0000 && dig<=0x000f)  result+=buf.sprintf("0%x",dig);
    if (dig>=0x0010 && dig<=0x00ff)  result+=buf.sprintf("%x",dig);
    return result.toUpper();
}
