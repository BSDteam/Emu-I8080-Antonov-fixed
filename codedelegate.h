#ifndef CODEDELEGATE_H
#define CODEDELEGATE_H

#include <QtGui>
#include "GLOBAL.h"


QT_FORWARD_DECLARE_CLASS(QPainter)


class CodeDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    CodeDelegate (QObject *parent);
    QWidget * createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index)const;
    QString displayText ( const QVariant & value, const QLocale & locale ) const;

signals:

public slots:

private:


};

#endif // CODEDELEGATE_H
