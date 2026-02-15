#include "LineEditBool.h"

LineEditBool::LineEditBool(QObject *parent) :
    QItemDelegate(parent)
{

}

QWidget * LineEditBool::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index)const
{
    QLineEdit *editor = new QLineEdit(parent);
    editor->setMaxLength(1);
    QIntValidator *qiv = new QIntValidator ( 0, 1, editor );
    editor->setValidator(qiv);
    editor->setEnabled(true);
    return editor;
}

