#include "mainwindow.h"

#include <QThread>

class I : public QThread
{
public:
        static void sleep(unsigned long secs) {
                QThread::sleep(secs);
        }
};


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    GL_MW=this;
    GL_TV=ui->tableView;
    aboutvs.setWindowIcon(QIcon(":img/screen.png"));
    ascii.setWindowIcon(QIcon(":img/ascii.png"));

    //создаем папку docs  если она не создана
    //QDir dir; $$ НЕ НАДО создавать папку по приколу
    //dir.mkdir("docs");

    //ассоцируем файлы (Не работает)
    QCheckBox * associate=new QCheckBox;
    #ifdef Q_WS_WIN
    {
        bool checked=false;
        QSettings reg("HKEY_CLASSES_ROOT", QSettings::NativeFormat);
        if(reg.value("/MPCCC.I80/shell/open/command/.").toString().contains("MPcccView.exe"))
            checked=true;
        associate->setChecked(checked);
        connect(associate, SIGNAL(stateChanged(int)),SLOT(slotAssociate(int)));
    }
    #else
        associate->setDisabled(true);
    #endif


    IsCodeEdit=false;
    IsFinish=false;
    IsEditCell=false;
    IsEditInstr=false;
    IsInput=false;

    this->setWindowTitle(QObject::trUtf8("Emu I8080 - обучающая программа"));
    this->setWindowIcon(QIcon(":img/prog.ico"));

    //настройка модели
    model=new QStandardItemModel();
    GL_MODEL=model;
    model->setHorizontalHeaderLabels(
    QStringList()<<"ADR"<<"INSRT"<<"CODE"
                );

    model->setRowCount(256*256);

    ui->tableView->setModel(model);
    ui->tableView->verticalHeader()->hide();
    ui->tableView->setItemDelegateForColumn(0,new AddressDelegate(this));
    ui->tableView->setItemDelegateForColumn(1,new InstrDelegate(this));
    ui->tableView->setItemDelegateForColumn(2,new CodeDelegate(this));
    ui->tableView->setColumnWidth(0,50);
    ui->tableView->setColumnWidth(1,270);
    ui->tableView->setColumnWidth(2,50);


    processor=new MICROPROC(ui->tableView,ui->textBrowser,this);
    connect(model,SIGNAL(dataChanged(QModelIndex,QModelIndex)),this,SLOT(MMM(QModelIndex,QModelIndex)));

    this->Reset();
    IsFinish=true;

    CurrIndex=4096;
    SetCurrenRow(CurrIndex); //1000 in HEX
    UpdateRegistrsAndFlags();

    //-------------------------------------------
    ui->mainToolBar->addWidget(ui->pushButton);
    ui->mainToolBar->addWidget(ui->pushButton_2);
    ui->mainToolBar->addWidget(ui->pushButton_3);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addWidget(ui->pushButton_11);
    ui->mainToolBar->addWidget(ui->pushButton_9);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addWidget(ui->pushButton_12);
    ui->mainToolBar->addWidget(ui->pushButton_10);
    //-------------------------------------------


    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(Run()));
    connect(ui->pushButton_2,SIGNAL(clicked()),this,SLOT(Step()));
    connect(ui->pushButton_3,SIGNAL(clicked()),this,SLOT(Stop()));
    connect(ui->pushButton_9,SIGNAL(clicked()),this,SLOT(CursorDown()));
    connect(ui->pushButton_11,SIGNAL(clicked()),this,SLOT(CursorUp()));
    connect(ui->pushButton_12,SIGNAL(clicked()),this,SLOT(Reset()));
    connect(ui->pushButton_10,SIGNAL(clicked()),this,SLOT(SetDefaultRegistr()));

    connect(ui->pushButton_4,SIGNAL(clicked()),this,SLOT(Go()));
    connect(ui->pushButton_8,SIGNAL(clicked()),this,SLOT(CursorSearch()));

    connect(ui->action_2,SIGNAL(triggered()),this,SLOT(open()));
    connect(ui->action_3,SIGNAL(triggered()),this,SLOT(save()));
    connect(ui->action_4,SIGNAL(triggered()),this,SLOT(save_as()));
    connect(ui->action_5,SIGNAL(triggered()),this,SLOT(AboutCommands()));
    connect(ui->action_19,SIGNAL(triggered()),this,SLOT(close()));

    connect(ui->action_7,SIGNAL(triggered()),this,SLOT(AboutProgramm()));
    connect(ui->action_6,SIGNAL(triggered()),this,SLOT(AboutAutor()));
    connect(ui->action_18,SIGNAL(triggered()),this,SLOT(AboutReg()));
    connect(ui->action_20,SIGNAL(triggered()),this,SLOT(AboutFlag()));

    connect(ui->action_9,SIGNAL(triggered()),this,SLOT(Run()));
    connect(ui->action_8,SIGNAL(triggered()),this,SLOT(Go()));
    connect(ui->action_14,SIGNAL(triggered()),this,SLOT(Step()));
    connect(ui->action_10,SIGNAL(triggered()),this,SLOT(Stop()));
    connect(ui->action_15,SIGNAL(triggered()),this,SLOT(Reset()));
    connect(ui->action_13,SIGNAL(triggered()),this,SLOT(CursorUp()));
    connect(ui->action_17,SIGNAL(triggered()),this,SLOT(CursorDown()));
    connect(ui->action_16,SIGNAL(triggered()),this,SLOT(CursorSearch()));
    connect(ui->action_12,SIGNAL(triggered()),this,SLOT(SetDefaultRegistr()));

    connect(ui->action_ASSCI,SIGNAL(triggered()),this,SLOT(AboutAscii()));
    connect(ui->action_VS,SIGNAL(triggered()),this,SLOT(AboutVSc()));

    ui->tableView->installEventFilter(this);

    ui->splitter->setStretchFactor(0,1);
    ui->splitter->setStretchFactor(1,0);
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
  {
    return false;
  }

//ассоциация
void MainWindow::slotAssociate(int state)
{
#ifdef Q_WS_WIN
    QSettings reg("HKEY_CLASSES_ROOT", QSettings::NativeFormat);
    if(state==Qt::Checked){
        reg.setValue("/.I80/.", "MPCCC.mpf");
        reg.setValue("/MPCCC.I80/DefaultIcon/.", QDir::toNativeSeparators(QCoreApplication::applicationDirPath())+"\\I80.ico");
        reg.setValue("/MPCCC.I80/shell/open/command/.", '"'+QDir::toNativeSeparators(QCoreApplication::applicationFilePath())+"\" \"%1\"");
    }else if(state==Qt::Unchecked){
        reg.setValue("/MPCCC.I80/shell/open/command/.", "");
    }
#endif
}


//перегружаем событие закрытия окна
void MainWindow::closeEvent(QCloseEvent *p)
{
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setWindowTitle(trUtf8("Предупреждение"));
    msgBox.setText(trUtf8("Вы уверены что хотите закрыть программу?"));
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    if(
        // QMessageBox::warning(this,
        //                  QObject::tr("Предупреждение"),
        //                  QObject::tr("Вы уверены что хотите закрыть программу?"),
        //                  QMessageBox::Ok|QMessageBox::Cancel
        //                  )
        //     !=QMessageBox::Ok
        msgBox.exec() !=QMessageBox::Ok
        )//если в диалоговом окне не была нажата кнопка Ok
    p->ignore();
    IsRun=false;
}


//открытие файла
void MainWindow::open()
{
    this->setEnabled(false);
    QString filename = QFileDialog::getOpenFileName(
                                                    this,
                                                    trUtf8("Открыть файл..."),
                                                    QDir::currentPath()+"/docs/",
                                                    trUtf8("Программа I8080 (*.I80)")
                                                    );
    if (filename=="")
    {
        this->setEnabled(true);
        return;

    }

    Reset();
    file_name=filename;

    QFile file(file_name);
    file.open(QIODevice::ReadOnly);
    QTextStream in(&file);

    int addr;
    QString code;

    in>>addr;
    this->SetCurrenRow(addr);
    while (!in.atEnd())
    {
        in>>addr;
        if(in.atEnd()) break;
        in>>code;
        if (addr>=256*256) continue;
        model->setData(model->index(addr,2),code,Qt::EditRole);
    }



    file.close();

    this->setEnabled(true);
}

//сохранить файл
void MainWindow::save()
{
    this->setEnabled(false);

    if (file_name=="")
    {
        save_as();
        this->setEnabled(true);
        return;
    }

    QFile file(file_name);
    file.open(QIODevice::WriteOnly);

    QTextStream out(&file);
    out<<processor->IP<<"\n";

    for (int i=0; i<256*256-1; i++)
        if (processor->memory[i]!=0)
        {
            out<<i<<" "<<QString::number(processor->memory[i],16)<<"\n";
        }

    file.close();

    QMessageBox::about(this,
                       QObject::trUtf8("Выполнено"),
                       QObject::trUtf8("Сохранение выполнено успешно...")
                       );

   this->setEnabled(true);
}

//задать для регистров и флагов значение по умолчанию
void MainWindow::SetDefaultRegistr()
{
    ui->tableWidget_3->item(0,1)->setText("0");
    ui->tableWidget_3->item(1,1)->setText("0");
    ui->tableWidget_3->item(1,2)->setText("0");
    ui->tableWidget_3->item(2,1)->setText("0");
    ui->tableWidget_3->item(2,2)->setText("0");
    ui->tableWidget_3->item(3,1)->setText("0");
    ui->tableWidget_3->item(3,2)->setText("0");

    ui->tableWidget_2->item(0,1)->setText("0");
    ui->tableWidget_2->item(1,1)->setText("0");
    ui->tableWidget_2->item(2,1)->setText("0");
    ui->tableWidget_2->item(3,1)->setText("0");
}

//очистить видио память
void MainWindow::ClearVideoMem()
{
    QString code="00";
    for (int i=0x0800; i<0x0800+32*10*2;i++)
    {
         model->setData(model->index(i,2),code,Qt::EditRole);
    }

}

//изменить цвет фона ячеек
void MainWindow::ChangeBgVideoMem()
{
    QColor bg;
    if(Items[0][0].backgroundColor()!=Qt::black)
        bg=Qt::black;
    else
        bg=Qt::white;

    for (int i=0; i<10; i++)
        for (int j=0; j<32; j++)
            Items[i][j].setBackgroundColor(bg);

}


//сохранить файл как
void MainWindow::save_as()
{

 QString   file_name_tmp = QFileDialog::getSaveFileName(
                                                    this,
                trUtf8("Сохранить файл..."),
                QDir::currentPath()+"/docs/",
                trUtf8("Программа I8080 (*.I80)")
            );
    if (file_name_tmp=="")return;

    this->setEnabled(false);

    file_name=file_name_tmp;

    save();

    this->setEnabled(true);
}


void MainWindow::UpTime()
{
    QString str=QString::number(processor->memory[inde_x],16);
    model->setData(model->index(inde_x,2),str,Qt::EditRole);

}

//Мы Можем Многое
void MainWindow::MMM(QModelIndex a,QModelIndex b)
{
    //если изменилась ячейка VS
    if (a.row()>=0x0800&&a.row()<=0x0800+32*10*2-1)
    {
        //получаем номер ячейки
        int tmp=a.row()-0x0800;

        //вычисляем ii,jj
        int ii=(tmp/2)/10;
        int jj=(tmp/2)-ii*10;


        if (tmp%2==0||tmp==0)
        {
            char ch=processor->memory[a.row()];
            Items[jj][ii].setText(VSCharEncoder(ch));
        }
        else
        {
            int col=processor->memory[a.row()];
            int r,g,b;
            r = (((unsigned char)(col & 0xE0)) >> 5) << 5;
            g = (((unsigned char)(col & 0x1C)) >> 2) << 5;
            b = (col & 3) << 6;
            Items[jj][ii].setTextColor(QColor(r,g,b));
        }
    }


    //если редактируем код
    if (a.column()==2&&!IsEditInstr&&!global_flag)
    {

        IsEditCell=true;

        processor->memory[a.row()]=a.data().toString().toInt(0,16);
        this->DisplayCommand(a.row(),0);


        IsEditCell=false;
        return;
    }


    if (a.column()==1&&IsFinish&&!IsEditCell)
    {
        IsEditInstr=true;

        IsEditInstr=false;
    }


}


void MainWindow::keyPressEvent(QKeyEvent *p)
{

    if (p->key()==90)
        this->Stop();
        p->accept();
}


void MainWindow::AboutAscii()
{

    ascii.show();
}

void MainWindow::AboutVSc()
{

    aboutvs.show();
}



//утановить IP
void MainWindow::SetCurrenRow(unsigned int row)
{
    QPalette pal = this->palette();

    model->setData(model->index(CurrIndex,0),QBrush( pal.color(QPalette::Base) ),Qt::BackgroundRole );
    model->setData(model->index(row,0),QBrush( QColor(0,255,0,90) ),Qt::BackgroundRole );
    ui->tableView->setCurrentIndex(model->index(row,0));
    CurrIndex=row;
    processor->IP=row;
    UpdateRegistrsAndFlags();
}


//переход на ячейку и перенос IP
void MainWindow::Go()
{
    if (ui->lineEdit->text()=="")return;
    int go=ui->lineEdit->text().toInt(0,16);
       if (go>256*256-1)return;
        SetCurrenRow(go);
        CursorSearch();
}

//инкремент IP
void MainWindow::CursorUp()
{
    if (CurrIndex-1<0)
        SetCurrenRow(256*256-1);
    else
        SetCurrenRow(CurrIndex-1);

}

//декремент IP
void MainWindow::CursorDown()
{
    if (CurrIndex+1==256*256)
        SetCurrenRow(0);
    else
        SetCurrenRow(CurrIndex+1);

}

//найти в таблице IP
void MainWindow::CursorSearch()
{
    ui->tableView->setCurrentIndex(model->index(CurrIndex,1));
    ui->tableView->setCurrentIndex(model->index(CurrIndex,0));
}


//Резет
void MainWindow::Reset()
{
    if(IsFinish)
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowTitle(trUtf8("Предупреждение"));
        msgBox.setText(trUtf8("Вы действительно хотите сбросить данные?"));
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        if(
            // QMessageBox::information(this, // ДО 2026 Года тут было this вместо nullptr
            //              QObject::trUtf8("Предупреждение"),
            //              QObject::trUtf8("Вы действительно хотите сбросить данные?"),
            //              QMessageBox::Ok|QMessageBox::Cancel
            //              )
            // !=QMessageBox::Ok
            // $$ Почему то это сообщение крашит все и вся, я (а я Гончаров) его переделал.
            msgBox.exec() !=QMessageBox::Ok)//если в диалоговом окне была нажата кнопка Ok
        return;
    }

    this->setEnabled(false);

    //VS
    //ui->graphicsView->setFixedSize(ui->graphicsView->size());
    //ui->tabWidget->setTabText(2,QObject::tr("Графический"));
    //ui->tabWidget->setTabText(3,QObject::tr("Текстовый 32x10"));

    //текстовый 32х10
    ui->tableWidget_4->setColumnCount(32);
    ui->tableWidget_4->setRowCount(10);
    //ui->tableWidget_4->verticalHeader()->close();
    //ui->tableWidget_4->horizontalHeader()->close();


    QFont font=Items[0][0].font();
    //font.setFamily("Courier");
    ui->tableWidget_4->setFont(font);
    //font.setPointSize(1);
    if (!IsFinish) { // Привязка массива элементов для Virtual Screen нужно проводить лишь единожды
        for (int i=0; i<10; i++)
        {
            for (int j=0; j<32; j++)
            {
                //Items[i][j].setBackgroundColor(Qt::black);
               // Items[i][j].setFont(font);
                ui->tableWidget_4->setItem(i,j,&Items[i][j]);
            }
        }
    }

    QFontMetrics fm = QFontMetrics(font);
#ifdef Q_WS_X11
    int charWidth = fm.charWidth(QString::fromUtf8("█"), 0) + 2;
#else
    int charWidth = fm.charWidth(QString::fromUtf8("█"), 0) + 6;
#endif
    int charHeight = fm.height();

    for (int j=0; j<32; j++)
        ui->tableWidget_4->setColumnWidth(j,charWidth);

    for (int j=0; j<10; j++)
        ui->tableWidget_4->setRowHeight(j,charHeight);
    QSize size = QSize(charWidth*32, charHeight*10);
    ui->tableWidget_4->setFixedSize(size);

    ui->tableWidget_4->updateGeometry();

    QSplashScreen *splash = new QSplashScreen(QPixmap(":/img/sfu.png"));
    splash->setWindowFlags( Qt::ToolTip);
    splash->showMessage(QObject::trUtf8("<font color=#fff><b>Siberian Federal University</b><br>ISIT<br>Кафедра ВТ, 2013</font><br><br><br><br><br><br><br><br><br><br><font color=#fff><h2>Идет построение таблицы...<h2></font>"),Qt::AlignLeft | Qt::AlignTop, Qt::black);
    splash->show();



    for (int i=0; i<256*256; i++)
    {



        QStandardItem * ItemAddr= new QStandardItem(QString::number(i));
        model->setItem(i,0,ItemAddr);

        QStandardItem * ItemInstr= new QStandardItem("");
        model->setItem(i,1,ItemInstr);

        QStandardItem * ItemCode= new QStandardItem("00");
        model->setItem(i,2,ItemCode);


        QApplication::processEvents();
    }

    if (!IsFinish)
    {

        I::sleep(2);
    }
    splash->close();

    IsReset=true;
    //таблица с регистрами и флагами
    ui->tableWidget_3->setItemDelegateForColumn(1,new CodeDelegate(this));
    ui->tableWidget_3->setItemDelegateForColumn(2,new CodeDelegate(this));

    ui->tableWidget_3->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->tableWidget_2->horizontalHeader()->setResizeMode(QHeaderView::Stretch);

    ui->tableWidget_3->verticalHeader()->hide();
    ui->tableWidget_2->verticalHeader()->hide();

    ui->tableWidget_3->setRowCount(5);
    ui->tableWidget_2->setRowCount(4);

    ui->tableWidget_3->setRowCount(6);

    //регистр А
    Item = new QTableWidgetItem(QObject::tr("A"));
    Item->setFlags(Qt::NoItemFlags|Qt::ItemIsEnabled);
    ui->tableWidget_3->setItem(0,0,Item);
    Item = new QTableWidgetItem(QObject::tr("BC"));
    Item->setFlags(Qt::NoItemFlags|Qt::ItemIsEnabled);
    ui->tableWidget_3->setItem(1,0,Item);
    Item = new QTableWidgetItem(QObject::tr("DE"));
    Item->setFlags(Qt::NoItemFlags|Qt::ItemIsEnabled);
    ui->tableWidget_3->setItem(2,0,Item);
    Item = new QTableWidgetItem(QObject::tr("HL"));
    Item->setFlags(Qt::NoItemFlags|Qt::ItemIsEnabled);
    ui->tableWidget_3->setItem(3,0,Item);
    Item = new QTableWidgetItem(QObject::tr("SP"));
    Item->setFlags(Qt::NoItemFlags|Qt::ItemIsEnabled);
    ui->tableWidget_3->setItem(4,0,Item);
    Item = new QTableWidgetItem(QObject::tr("IP"));
    Item->setFlags(Qt::NoItemFlags|Qt::ItemIsEnabled);
    ui->tableWidget_3->setItem(5,0,Item);

    Item = new QTableWidgetItem(QObject::tr(""));
    Item->setFlags(Qt::NoItemFlags|Qt::ItemIsEnabled);
    ui->tableWidget_3->setItem(0,2,Item);


    Item = new QTableWidgetItem(QObject::tr("00"));
    ui->tableWidget_3->setItem(0,1,Item);
    Item = new QTableWidgetItem(QObject::tr("00"));
    ui->tableWidget_3->setItem(1,1,Item);
    Item = new QTableWidgetItem(QObject::tr("00"));
    ui->tableWidget_3->setItem(2,1,Item);
    Item = new QTableWidgetItem(QObject::tr("00"));
    ui->tableWidget_3->setItem(3,1,Item);

    Item = new QTableWidgetItem;
    Item->setFlags(Qt::NoItemFlags|Qt::ItemIsEnabled);
    ui->tableWidget_3->setItem(4,1,Item);



    Item = new QTableWidgetItem;
    Item->setFlags(Qt::NoItemFlags|Qt::ItemIsEnabled);
    ui->tableWidget_3->setItem(5,1,Item);

    Item = new QTableWidgetItem(QObject::tr("00"));
    ui->tableWidget_3->setItem(1,2,Item);
    Item = new QTableWidgetItem(QObject::tr("00"));
    ui->tableWidget_3->setItem(2,2,Item);
    Item = new QTableWidgetItem(QObject::tr("00"));
    ui->tableWidget_3->setItem(3,2,Item);

    Item = new QTableWidgetItem;
    Item->setFlags(Qt::NoItemFlags|Qt::ItemIsEnabled);
    ui->tableWidget_3->setItem(4,2,Item);


    Item = new QTableWidgetItem;
    Item->setFlags(Qt::NoItemFlags|Qt::ItemIsEnabled);
    ui->tableWidget_3->setItem(5,2,Item);

    ui->tableWidget_3->setMinimumSize(0,220);

    Item = new QTableWidgetItem(QObject::tr("C"));
    Item->setFlags(Qt::NoItemFlags|Qt::ItemIsEnabled);
    ui->tableWidget_2->setItem(0,0,Item);
    Item = new QTableWidgetItem(QObject::tr("Z"));
    Item->setFlags(Qt::NoItemFlags|Qt::ItemIsEnabled);
    ui->tableWidget_2->setItem(1,0,Item);
    Item = new QTableWidgetItem(QObject::tr("P"));
    Item->setFlags(Qt::NoItemFlags|Qt::ItemIsEnabled);
    ui->tableWidget_2->setItem(2,0,Item);
    Item = new QTableWidgetItem(QObject::tr("S"));
    Item->setFlags(Qt::NoItemFlags|Qt::ItemIsEnabled);
    ui->tableWidget_2->setItem(3,0,Item);

    Item = new QTableWidgetItem(QObject::tr("0"));
    ui->tableWidget_2->setItem(0,1,Item);
    Item = new QTableWidgetItem(QObject::tr("0"));
    ui->tableWidget_2->setItem(1,1,Item);
    Item = new QTableWidgetItem(QObject::tr("0"));
    ui->tableWidget_2->setItem(2,1,Item);
    Item = new QTableWidgetItem(QObject::tr("0"));
    ui->tableWidget_2->setItem(3,1,Item);

    ui->tableWidget_3->setFocusPolicy(Qt::NoFocus);
    ui->tableWidget_2->setFocusPolicy(Qt::NoFocus);


    ui->tableWidget_2->setItemDelegate(new LineEditBool);
    ui->tableWidget_3->setItemDelegate(new LineEditHex);

  //  ui->tableWidget->setItemDelegateForColumn(2,new LineEditHex);

    delete processor;
    processor = new MICROPROC(ui->tableView,ui->textBrowser,this);

    //prees f9
    for(unsigned int i=0;i<MEMSIZE;i++)
      processor->memory[i]=0;

    processor->IP=4096;

    //prees_s f5
    processor->A=0;
    processor->B=0;
    processor->C=0;
    processor->D=0;
    processor->E=0;
    processor->H=0;
    processor->L=0;
    processor->flag.Z=0;
    processor->flag.C=0;
    processor->flag.P=0;
    processor->flag.S=0;
    processor->IP=Default_IP;
    processor->SP=Default_SP;
    PP=processor->IP;
    ON=0;



    for(int i=0;i<320;i++) // clear txt-video memory
    processor->memory[0x0800+i]=0;
    PP=processor->IP-8;

    QCoreApplication::processEvents();

    IsReset=false;

    UpdateRegistrsAndFlags();
    this->setEnabled(true);
    SetCurrenRow(4096);
}




void MainWindow::DisplayCommand(unsigned int index,bool ignore)
{

    //получаем команду
    int code=processor->memory[index];

    //если это VS
    if (index>=0x0800&&index<=0x0800+32*10*2-1)
    {
        QStandardItem *VsItem= new QStandardItem ( QString("Virtual Screen") );
        VsItem->setBackground(QBrush(QColor(0,255,255,80)));
        VsItem->setEditable(false);
        model->setItem(index,1,VsItem);
        return;

    }




    //проверяем команда ли это (если это часть команды то выходим)
    if(!ignore&&model->index(index,1).data(Qt::EditRole).toString()==QString("^^^"))
    {
        DisplayCommand(index-1,false);
        return;
    }


    int len =3;

    //проверяем сколько в ней бит 1,2 или 3

    for (int i=0; i<len_3_count; i++)
    {
        if ( code==len_3[i])
        {
            goto go;
        }
    }

    len--;

    for (int i=0; i<len_2_count; i++)
    {
        if ( code==len_2[i])
        {
            goto go;
        }
    }

    len--;

    go:


    //разблокировка ячеек
    if (len==1&&model->index(index+1,1).data(Qt::EditRole).toString()==QString("^^^"))
    {
        DisplayCommand(index+1,true);
    }

    if (len==2&&model->index(index+2,1).data(Qt::EditRole).toString()==QString("^^^"))
    {
        DisplayCommand(index+2,true);
    }

    if (len==3&&model->index(index+3,1).data(Qt::EditRole).toString()==QString("^^^"))
    {
        DisplayCommand(index+3,true);
    }



    //в зависимости от количества бит обновляем ячейки
    QStandardItem *SItem,*BlockItem;
    QString s1,s2;


    switch (len)
    {
        case 1:
        {
        SItem=new QStandardItem (QString(comm[code])) ;
        model->setItem(index,1,SItem);
        SItem->setBackground(QBrush(QColor(0,100,255,30)));
        break;
        }

        case 2:
        {
        QString s1=MICROPROC::put_hex2(processor->memory[index+1]);

        SItem=new QStandardItem ( QString(comm[code]).replace('%',s1)) ;
        SItem->setBackground(QBrush(QColor(0,100,255,30)));
        model->setItem(index,1,SItem);

        if (index+1>=0x0800&&index+1<=0x0800+32*10*2-1)return;
        BlockItem= new QStandardItem ( QString("^^^") );
        BlockItem->setBackground(QBrush(QColor(255,0,0,30)));
        BlockItem->setEditable(false);
        model->setItem(index+1,1,BlockItem);

        break;
        }


        case 3:
        {
        s1=MICROPROC::put_hex2(processor->memory[index+1]);
        s2=MICROPROC::put_hex2(processor->memory[index+2]);

        SItem=new QStandardItem (  QString(comm[code]).replace('%',s2+s1)) ;
        SItem->setBackground(QBrush(QColor(0,100,255,30)));
        model->setItem(index,1,SItem);

        if (index+1>=0x0800&&index+1<=0x0800+32*10*2-1)return;
        BlockItem= new QStandardItem ( QString("^^^") );
        BlockItem->setBackground(QBrush(QColor(255,0,0,30)));
        BlockItem->setEditable(false);
        model->setItem(index+1,1,BlockItem);

        if (index+2>=0x0800&&index+2<=0x0800+32*10*2-1)return;
        BlockItem= new QStandardItem ( QString("^^^") );
        BlockItem->setBackground(QBrush(QColor(255,0,0,30)));
        BlockItem->setEditable(false);
        model->setItem(index+2,1,BlockItem);

        break;
        }
    }


    if (len!=1)
        DisplayCommand(index+len,true);


}

//перевод числа в 16-ричную СС (4 разряда)
QString MainWindow::put_hex4(unsigned int dig)
{
 QString result,buf;
 if (dig>=0x0000 && dig<=0x000f)
  result+=buf.sprintf("000%x",dig);
 if (dig>=0x0010 && dig<=0x00ff)
  result+=buf.sprintf("00%x",dig);
 if (dig>=0x0100 && dig<=0x0fff)
  result+=buf.sprintf("0%x",dig);
 if (dig>=0x1000 && dig<=0xffff)
  result+=buf.sprintf("%x",dig);
 return result.toUpper();
}


//переводим числа в 16-ричную СС (2 разряда)
QString MainWindow::put_hex2(unsigned int dig)
{
 QString result,buf;
 if (dig>=0x0000 && dig<=0x000f)  result+=buf.sprintf("0%x",dig);
 if (dig>=0x0010 && dig<=0x00ff)  result+=buf.sprintf("%x",dig);
 return result.toUpper();
}



//изменяем регистры
void  MainWindow::EditReg(int row, int column)
{

    if (ui->tableWidget_3->item(row,column)->text()=="")
    {
        ui->tableWidget_3->item(row,column)->setText(LastString);
    }

    if (row==0&&column==1)
    {
        processor->A=ui->tableWidget_3->item(row,column)->text().toInt(0,16);

    }

    if (row==1&&column==1)
    {
        processor->B=ui->tableWidget_3->item(row,column)->text().toInt(0,16);
    }

    if (row==2&&column==1)
    {
        processor->D=ui->tableWidget_3->item(row,column)->text().toInt(0,16);
    }

    if (row==3&&column==1)
    {
        processor->H=ui->tableWidget_3->item(row,column)->text().toInt(0,16);
    }

    if (row==1&&column==2)
    {
        processor->C=ui->tableWidget_3->item(row,column)->text().toInt(0,16);
    }

    if (row==2&&column==2)
    {
        processor->E=ui->tableWidget_3->item(row,column)->text().toInt(0,16);
    }

    if (row==3&&column==2)
    {
        processor->L=ui->tableWidget_3->item(row,column)->text().toInt(0,16);
    }

}

//начали изменять регистры
void  MainWindow::StartEditReg(int row, int column)
{
    LastString=ui->tableWidget_3->item(row,column)->text();
}

void MainWindow::DisableGroupRun()
{
    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setEnabled(false);
    ui->pushButton_9->setEnabled(false);
    ui->pushButton_11->setEnabled(false);
    ui->pushButton_12->setEnabled(false);
    ui->pushButton_10->setEnabled(false);
    ui->pushButton_6->setEnabled(false);
    ui->pushButton_4->setEnabled(false);
    ui->pushButton_8->setEnabled(false);

    ui->action_2->setEnabled(false);
    ui->action_3->setEnabled(false);
    ui->action_4->setEnabled(false);
    ui->action_8->setEnabled(false);
    ui->action_9->setEnabled(false);
    ui->action_14->setEnabled(false);
    ui->action_12->setEnabled(false);
    ui->action_15->setEnabled(false);
    ui->action_13->setEnabled(false);
    ui->action_16->setEnabled(false);
    ui->action_17->setEnabled(false);
}

void MainWindow::EnebleGroupRun()
{
    ui->pushButton->setEnabled(true);
    ui->pushButton_2->setEnabled(true);
    ui->pushButton_9->setEnabled(true);
    ui->pushButton_11->setEnabled(true);
    ui->pushButton_12->setEnabled(true);
    ui->pushButton_10->setEnabled(true);
    ui->pushButton_6->setEnabled(true);
    ui->pushButton_4->setEnabled(true);
    ui->pushButton_8->setEnabled(true);

    ui->action_2->setEnabled(true);
    ui->action_3->setEnabled(true);
    ui->action_4->setEnabled(true);
    ui->action_8->setEnabled(true);
    ui->action_9->setEnabled(true);
    ui->action_14->setEnabled(true);
    ui->action_15->setEnabled(true);
    ui->action_13->setEnabled(true);
    ui->action_16->setEnabled(true);
    ui->action_17->setEnabled(true);
    ui->action_12->setEnabled(true);
}


//изменение флагов
void MainWindow::EditFlag(int row, int column)
{

    if (ui->tableWidget_2->item(row,column)->text()=="")
    {
        ui->tableWidget_2->item(row,column)->setText(LastString);
        return;
    }

    if (row==0&&column==1)
    {
        processor->flag.C=ui->tableWidget_2->item(row,column)->text().toInt(0,2);
    }

    if (row==1&&column==1)
    {
        processor->flag.Z=ui->tableWidget_2->item(row,column)->text().toInt(0,2);
    }


    if (row==2&&column==1)
    {
         processor->flag.P=ui->tableWidget_2->item(row,column)->text().toInt(0,2);
    }

    if (row==3&&column==1)
    {
         processor->flag.S=ui->tableWidget_2->item(row,column)->text().toInt(0,2);
    }

}

void MainWindow::UpdateTimeInfo()
{
    sec++;
    ui->textBrowser_2->clear();
    ui->textBrowser_2->insertHtml(QObject::trUtf8("<font color=green><strong>Информация о последнем запуске</strong></font><br>"));
    ui->textBrowser_2->insertHtml(QObject::trUtf8("<font color=green>Время работы: </font>")+QString::number(sec)+" sec<br>");
    ui->textBrowser_2->insertHtml(QObject::trUtf8("<font color=green>Выполненные команды:</font> ")+QString::number(step)+"<br>");
    QApplication::processEvents();
}

//останов
void MainWindow::Stop()
{
    UpdateTimeInfo();
    ui->tableView->setUpdatesEnabled(true);
    ui->tableWidget_3->setUpdatesEnabled(true);
    ui->tableWidget_2->setUpdatesEnabled(true);
    QCoreApplication::processEvents();
    IsRun=false;
    QCoreApplication::processEvents();
    QCoreApplication::processEvents();
    QCoreApplication::processEvents();
    QCoreApplication::processEvents();
    EnebleGroupRun();
}

//запуск
void MainWindow::Run()
{
    sec=0;
    step=0;
    QTimer Vp;

    DisableGroupRun();
    //выключаем обновление таблиц
    ui->tableView->setUpdatesEnabled(false);
    ui->tableWidget_3->setUpdatesEnabled(false);
    ui->tableWidget_2->setUpdatesEnabled(false);

    connect(&Vp,SIGNAL(timeout()),this,SLOT(UpdateTimeInfo()));
    Vp.start(1000);

    int i=0;

    IsRun=true;
    while (IsRun)
    {
        i++;
        if (i>1000)
        {
            QCoreApplication::processEvents();
            i=0;
        }

        //зацикливаем
        if (CurrIndex>=256*256-1)SetCurrenRow(0);

        //делаем шаг
        Step();

        //если останов
        if (processor->memory[processor->IP]==0x76)
            Stop();
    }

    Vp.stop();
 }


//обновить регисты
void MainWindow::UpdateRegistrsAndFlags()
{
    //регисты
    ui->tableWidget_3->item(0,1)->setText(QString::number(processor->A,16));
    ui->tableWidget_3->item(1,1)->setText(QString::number(processor->B,16));
    ui->tableWidget_3->item(1,2)->setText(QString::number(processor->C,16));
    ui->tableWidget_3->item(2,1)->setText(QString::number(processor->D,16));
    ui->tableWidget_3->item(2,2)->setText(QString::number(processor->E,16));
    ui->tableWidget_3->item(3,1)->setText(QString::number(processor->H,16));
    ui->tableWidget_3->item(3,2)->setText(QString::number(processor->L,16));
    ui->tableWidget_3->item(4,1)->setText(QString::number(processor->SP,16));
    ui->tableWidget_3->item(5,1)->setText(QString::number(processor->IP,16));

    //флаги
    ui->tableWidget_2->item(0,1)->setText(QString::number(processor->flag.C));
    ui->tableWidget_2->item(1,1)->setText(QString::number(processor->flag.Z));
    ui->tableWidget_2->item(2,1)->setText(QString::number(processor->flag.P));
    ui->tableWidget_2->item(3,1)->setText(QString::number(processor->flag.S));
}

//сделать шаг
void MainWindow::Step()
{
    step++;
    //PP=processor->IP-8;
    processor->alu(processor->memory[processor->IP]);
    SetCurrenRow(processor->IP);

}


//если начали изменять флаги
void MainWindow::StartEditFlag(int row, int column)
{
    LastString=ui->tableWidget_2->item(row,column)->text();
}

// о программе
void MainWindow::AboutProgramm()
{
    QMessageBox::about(this,
                       QObject::trUtf8("О программе"),
                       QObject::trUtf8("Эмулятор  I8080(K580)\nРевизия 1.3\nЭта программа является учебной. Разработана студентом кафедры ВТ, Института Космических и Информационных Технологий Сибирского Федерального Университета (ИКИТ СФУ)\nНаучный руководитель: Середкин Вениамин Георгиевич\nГончаров И.Е. исправлял здесь ошибки в 2026 :)"));
}

//о регистрах
void MainWindow::AboutReg()
{
    QMessageBox::about(this,
                       QObject::trUtf8("Регистры"),
                       QObject::trUtf8("<b>A</b> – аккумулятор. Все арифметические и логические операции производятся только между A и другими регистрами или между A и байтом непосредственных данных.<hr>")
                       + QObject::trUtf8("<b>B, C, D, E, H, L</b> – 8-разрядные регистры общего назначения.<hr>")
                       + QObject::trUtf8("<b>HL</b> – регистровая пара, состоящая из двух 8-разрядных регистров ( H – старший регистр, L – младший), используется для косвенно-регистровой адресации 64 Кбайт памяти.<hr>")
                       + QObject::trUtf8("<b>DE</b> – часто используемая регистровая пара, поскольку имеется команда обмена содержимым между парами HL и DE .<hr>")
                        + QObject::trUtf8("<b>IP</b> – счётчик команд, содержит адрес очередной исполняемой команды.<hr>")
                       + QObject::trUtf8("<b>SP</b> – указатель стека автоматически инкрементируется на 2 при записи пары регистров в стек (отдельный 8-разрядный регистр в стек записать нельзя, только парами) и декрементируется при извлечении из регистровой пары из стека.<hr>")
                       + QObject::trUtf8("<b>F</b> – регистр флагов. Непосредственно недоступен программисту, но его в составе PSW можно сохранить в стеке, а потом извлечь в другую регистровую пару, если нужно специально установить или проверить нужные флаги.")

                       );
}


// о флагах
void MainWindow::AboutFlag()
{
    QMessageBox::about(this,
                       QObject::trUtf8("Флаги"),
                       QObject::trUtf8("<b>S</b> – знак <hr>")
                       +QObject::trUtf8("<b>Z</b> – нуль <hr>")
                       +QObject::trUtf8("<b>P</b> – чётность <hr>")
                       +QObject::trUtf8("<b>С</b> – перенос")

                       );
}

// $$ об отсутствии справки по командам
void MainWindow::AboutCommands()
{
    QMessageBox::about(this,
                       QObject::trUtf8("Справки тут нет"),
                       QObject::trUtf8("Антонов О.И. просто <i>забыл</i> разместить справку тут<br>")
                           +QObject::trUtf8("По крайней мере, я ее не нашел<br>")
                           +QObject::trUtf8("Но инструкции здесь выглядят так<hr>")
                           +QObject::trUtf8("<b>MVI A,01</b><hr>")
                            +QObject::trUtf8("Ставится пробел между командой и аргументами<br>")
                            +QObject::trUtf8("Между аргументами запятая без пробела")

                       );
}


//об авторе
void MainWindow::AboutAutor()
{
    QMessageBox::about(this,
                       QObject::trUtf8("Об авторе"),
                       QObject::trUtf8("Программу написал студент кафедры ВТ Института Космических и Информационных Технологий Сибирского Федерального Университета (ИКИТ СФУ)\n\tАнтонов О.И.\n\tvk.com/order83\n\ttheorder83@gmail.com\nНаучный руководитель:Середкин В.Г.\n\nИсправлял программу Гончаров И.Е. 2026\n tg @Sunrise_ne_yveren"));
}


//деструктор
MainWindow::~MainWindow()
{
    delete ui;
}
