#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include <QStack>
#include "I8080.h"
#include "ui_mainwindow.h"
#include "LineEditBool.h"
#include "lineedithex.h"

#include "GLOBAL.h"
#include "INPUT.h"
#include "output.h"
#include "ascii.h"
#include "aboutvs.h"

//делигаты
#include "addressdelegate.h"
#include "instrdelegate.h"
#include "codedelegate.h"

#include "VSCharEncoder.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:

    void closeEvent(QCloseEvent *p);
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
        void SetCurrenRow (unsigned int row);
private:
    bool eventFilter(QObject *obj, QEvent *event);
    //count step
    unsigned long long step;
    //count sec
    unsigned long long sec;
    Ui::MainWindow *ui;
    Ascii ascii;
    AboutVS aboutvs;
    int inde_x;
    QString old_str;
    int old_code;
    QString LastString;
    QString file_name;
    QStandardItemModel *model;
    QProgressBar *pBar;

    QTableWidgetItem *Item;

    QTableWidgetItem Items[10][32];
    int CurrIndex;
    bool IsRun;
    bool IsCodeEdit;
    bool IsInstrEdit;
    bool IsReset;
    bool IsFinish;
    bool IsEditCell;
    bool IsEditInstr;
    bool IsInput;


    QString put_hex4(unsigned int dig);
    QString put_hex2(unsigned int dig);
    QString hex2num (QString str);
    void UpdateRegistrsAndFlags();
    QGraphicsScene *scene;
    int curr_row;

    void keyPressEvent(QKeyEvent *p);

    struct ProcMem
    {
        int len;//count bit
        int bit[3];//bits
    } CellMem;

private slots:
    void open();
    void save_as();
    void save();

    void AboutProgramm();
    void AboutAutor();
    void AboutReg();
    void AboutFlag();
    void AboutAscii();
    void AboutVSc();
    void AboutCommands();
    void Go();
    void CursorUp();
    void CursorDown();
    void CursorSearch();
    void Reset();
    void Step();
    void Run();
    void Stop();

    void EditReg(int row, int column);
    void StartEditReg(int row, int column);

    void EditFlag(int row, int column);
    void StartEditFlag(int row, int column);


    void ClearVideoMem();
    void ChangeBgVideoMem();
    void SetDefaultRegistr();
    void MMM(QModelIndex a,QModelIndex b);
    void slotAssociate(int state);
    void UpdateTimeInfo();
    void DisableGroupRun();
    void EnebleGroupRun();
    void UpTime();
public slots:
    void DisplayCommand(unsigned int index, bool ignore);

};

#endif // MAINWINDOW_H
