#ifndef ABOUTVS_H
#define ABOUTVS_H

#include <QDialog>

namespace Ui {
class AboutVS;
}

class AboutVS : public QDialog
{
    Q_OBJECT
    
public:
    explicit AboutVS(QWidget *parent = 0);
    ~AboutVS();
    
private:
    QString put_hex2(unsigned int dig);
    Ui::AboutVS *ui;
};

#endif // ABOUTVS_H
