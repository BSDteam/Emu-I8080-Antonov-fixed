#include "aboutvs.h"
#include "ui_aboutvs.h"


QString AboutVS::put_hex2(unsigned int dig)
{
 QString result,buf;
 if (dig>=0x0000 && dig<=0x000f)  result+=buf.sprintf("0%x",dig);
 if (dig>=0x0010 && dig<=0x00ff)  result+=buf.sprintf("%x",dig);
 return result.toUpper();
}


AboutVS::AboutVS(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutVS)
{
    ui->setupUi(this);

    //
    this->setWindowTitle("Virtual Screen info");
    ui->tableWidget->setRowCount(256);

    for (int i=0; i<256; i++)
    {
        QTableWidgetItem *Item=new QTableWidgetItem ();
        int col=i;
        int r,g,b;
        r = (((unsigned char)(col & 0xE0)) >> 5) << 5;
        g = (((unsigned char)(col & 0x1C)) >> 2) << 5;
        b = (col & 3) << 6;
        Item->setBackgroundColor(QColor(r,g,b));
        ui->tableWidget->setItem(i,2,Item);
        Item=new QTableWidgetItem (put_hex2(i));
        ui->tableWidget->setItem(i,1,Item);
        Item=new QTableWidgetItem (QString::number(i));
        ui->tableWidget->setItem(i,0,Item);
    }

}

AboutVS::~AboutVS()
{
    delete ui;
}
