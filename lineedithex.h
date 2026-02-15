#ifndef LINEEDITHEX_H
#define LINEEDITHEX_H

#include <QtGui>

class LineEditHex : public  QItemDelegate
{
    Q_OBJECT

public:
    explicit LineEditHex(QObject *parent = 0);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                            const QModelIndex &index) const;
signals:

public slots:

};

#endif
