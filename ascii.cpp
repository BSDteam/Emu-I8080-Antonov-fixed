#include "ascii.h"
#include "ui_ascii.h"

QString Ascii::put_hex2(unsigned int dig)
{
 QString result,buf;
 if (dig>=0x0000 && dig<=0x000f)  result+=buf.sprintf("0%x",dig);
 if (dig>=0x0010 && dig<=0x00ff)  result+=buf.sprintf("%x",dig);
 return result.toUpper();
}


Ascii::Ascii(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Ascii)
{
    ui->setupUi(this);
    this->setWindowTitle(QObject::tr("ASCII table"));
    ui->tableWidget->setRowCount(256);

    char ch=0;
    for (int i=0; i<256; i++)
    {
        ui->tableWidget->setRowHeight(i,50);

        QTableWidgetItem *Item=new QTableWidgetItem (QString(QString(ch).toAscii()));
        ch++;
        QFont font=Item->font();
        font.setPointSize(15);
        Item->setFont(font);
        ui->tableWidget->setItem(i,2,Item);
        Item=new QTableWidgetItem (put_hex2(i));

         Item->setFont(font);
        ui->tableWidget->setItem(i,1,Item);

        Item=new QTableWidgetItem (QString::number(i));
         Item->setFont(font);
        ui->tableWidget->setItem(i,0,Item);
    }


}

Ascii::~Ascii()
{
    delete ui;
}
