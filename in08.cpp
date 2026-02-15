#include "in08.h"
#include "ui_in08.h"

in08::in08(QDialog *parent) :
    QDialog(parent),
    ui(new Ui::in08)
{
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint  );


    editor= new QLineEdit(this);

    QRegExp *rx = new QRegExp ("[0-9,a-f,A-F]{1,2}");
    QRegExpValidator *qiv = new QRegExpValidator (*rx,editor);
    editor->setValidator(qiv);
    editor->setEnabled(true);
    editor->setGeometry(30,8,150,20);
    ui->setupUi(this);
}

in08::~in08()
{
    delete ui;
}


void in08::Push()
{
    QString buf=editor->text();
    buf=buf.remove(' ');
    buf=buf.remove('    ');
    if (buf=="") return;
    this->value= editor->text().toInt(0,16);
    this->editor->clear();
    this->close();

}
