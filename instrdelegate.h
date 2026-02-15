#ifndef INSTRDELEGATE_H
#define INSTRDELEGATE_H

#include <QtGui>
#include "GLOBAL.h"
#include "INPUT.h"

QT_FORWARD_DECLARE_CLASS(QPainter)


class InstrDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    InstrDelegate (QObject *parent);


    QString displayText ( const QVariant & value, const QLocale & locale ) const;

    void setModelData(QWidget *editor,
    QAbstractItemModel *model, const QModelIndex &index) const;

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
    const QModelIndex &index) const; //создаем редактор делегата - это наш виджет
    void setEditorData(QWidget *editor, const QModelIndex &index) const; //устанавливаем данные в редакторе

signals:

public slots:

private:
    QAbstractItemModel *LastModel;
    QModelIndex LastIndex;

};

#endif // INSTRDELEGATE_H
