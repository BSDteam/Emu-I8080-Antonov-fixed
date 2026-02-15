#include <QtGui>
#include "mainwindow.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //для отображения кирилицы
    //QTextCodec::setCodecForTr(QTextCodec::codecForName("Windows-1251")); // $$ По-хорошему, надо было бы сразу использовать UTF-8, соответственно везде, где в коде было tr(), надо было писать сразу trUtf8()

    MainWindow w;
    w.setWindowIcon(QIcon(":img/intel.png"));
    w.show();
    

    return a.exec();
}
