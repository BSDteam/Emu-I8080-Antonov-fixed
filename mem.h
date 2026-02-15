#ifndef MEM_H
#define MEM_H
#include <Qtgui>
#include <QTableWidget>
#include "mem_cell.h"


class mem
{
private:
    unsigned char *data;
    unsigned char *command;
    QTableView *table;
    bool *isActiv;
public:
    mem();
    QString put_hex2(unsigned int dig);
    void SetFlag(bool &fl);
    void SetTable (QTableView *t);
    unsigned char& operator[]( int index);

};

#endif // MEM_H
