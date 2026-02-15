
#include "lineedithex.h"

LineEditHex::LineEditHex(QObject *parent) :
    QItemDelegate(parent)
{

}

QWidget * LineEditHex::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index)const
{
    QLineEdit *editor = new QLineEdit(parent);

    QRegExp *rx = new QRegExp ("[0-9,a-f,A-F]{1,2}");
    QRegExpValidator *qiv = new QRegExpValidator (*rx,editor);
    editor->setValidator(qiv);
    editor->setEnabled(true);
    return editor;
}


