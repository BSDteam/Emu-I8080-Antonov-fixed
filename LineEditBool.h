#ifndef LINEEDITBOOL_H
#define LINEEDITBOOL_H

#include <QtGui>

class LineEditBool : public  QItemDelegate
{
    Q_OBJECT

public:
    explicit LineEditBool(QObject *parent = 0);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                            const QModelIndex &index) const;
signals:
    
public slots:
    
};

#endif
