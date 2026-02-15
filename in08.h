#ifndef IN08_H
#define IN08_H

#include <QDialog>
#include "lineedithex.h"

namespace Ui {
class in08;
}

class in08 : public QDialog
{
    Q_OBJECT
    
public:
    explicit in08(QDialog *parent = 0);
    ~in08();
    int getValue()
    {
        return this->value;
    }

private:
     QLineEdit  * editor;
    int value;
    Ui::in08 *ui;
private slots:
    void Push();


};

#endif // IN08_H
