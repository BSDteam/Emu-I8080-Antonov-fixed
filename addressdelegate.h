#ifndef ADDRESSDELEGATE_H
#define ADDRESSDELEGATE_H

#include <QtGui>


QT_FORWARD_DECLARE_CLASS(QPainter)


class AddressDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    AddressDelegate (QObject *parent);


    QString displayText ( const QVariant & value, const QLocale & locale ) const;

    QWidget* AddressDelegate::createEditor ( QWidget*, const QStyleOptionViewItem&, const QModelIndex& ) const;


signals:
    
public slots:
    
};

#endif // ADDRESSDELEGATE_H
