#include <QtGui>
#include "mainwindow.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //для отображения кирилицы
    QTextCodec::setCodecForTr(QTextCodec::codecForName("Windows-1251"));


    MainWindow w;
    w.setWindowIcon(QIcon(":img/intel.png"));
    w.show();
    

    return a.exec();
}
