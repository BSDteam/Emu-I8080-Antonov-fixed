#include "codedelegate.h"

CodeDelegate::CodeDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{

}


QWidget * CodeDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index)const
{
    QLineEdit *editor = new QLineEdit(parent);
    QRegExp *rx = new QRegExp ("[0-9,a-f,A-F]{1,2}");
    QRegExpValidator *qiv = new QRegExpValidator (*rx,editor);
    editor->setValidator(qiv);
    editor->setEnabled(true);
    return editor;
}


QString CodeDelegate::displayText ( const QVariant & value, const QLocale & locale ) const
{
    QString str=value.toString();
    if (str.length()==1)
        return '0'+str;
  return str.toUpper();
}
