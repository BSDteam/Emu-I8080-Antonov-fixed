#ifndef ASCII_H
#define ASCII_H

#include <QDialog>
#include "VSCharEncoder.h"

namespace Ui {
class Ascii;
}

class Ascii : public QDialog
{
    Q_OBJECT
    
public:
    explicit Ascii(QWidget *parent = 0);
    QString put_hex2(unsigned int dig);
    ~Ascii();
    
private:
    Ui::Ascii *ui;
};

#endif // ASCII_H
