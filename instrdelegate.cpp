#include "instrdelegate.h"




InstrDelegate::InstrDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{

}


void InstrDelegate::setModelData(QWidget *editor,
 QAbstractItemModel *model, const QModelIndex &index) const
{
 //сюда попадаем когда редактор делегата теряем фокус/закрывается
 QLineEdit *leditor = static_cast<QLineEdit*> (editor);
 QString qstring=leditor->text();
 char * str=new char [256];
 strcpy (str,qstring.toLocal8Bit().data());
 input(str,index.row(),model);

 model->setData(model->index(index.row(),index.column()),GL_STR); //устанавливаем новое значение в модель
 GL_STR="NOP";





}

QWidget *InstrDelegate::createEditor(QWidget *parent,
 const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QLineEdit *dlg = new QLineEdit (parent); //создаем наше поле ввода с кнопкой
    return dlg;
}

void InstrDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
     //в этой функции на входе данные из модели и указатель на виджет редактора
     QString value = index.model()->data(index).toString(); //получаем что уже есть в модели
     GL_STR=value;
     QLineEdit *fileDialog = static_cast<QLineEdit*> (editor); //преобразуем указатель
     fileDialog->setText(value); //устанавливаем текст
}

QString InstrDelegate::displayText ( const QVariant & value, const QLocale & locale ) const
{
    QString str=value.toString();
    return QStyledItemDelegate::displayText(str+"",locale);
}


