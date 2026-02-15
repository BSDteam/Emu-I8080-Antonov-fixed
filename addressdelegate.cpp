#include "addressdelegate.h"

AddressDelegate::AddressDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{

}

QString AddressDelegate::displayText ( const QVariant & value, const QLocale & locale ) const
{
    int dig=value.toString().toInt();
    QString result,buf;
    if (dig>=0x0000 && dig<=0x000f)
     result+=buf.sprintf("000%x",dig);
    if (dig>=0x0010 && dig<=0x00ff)
     result+=buf.sprintf("00%x",dig);
    if (dig>=0x0100 && dig<=0x0fff)
     result+=buf.sprintf("0%x",dig);
    if (dig>=0x1000 && dig<=0xffff)
     result+=buf.sprintf("%x",dig);

    return result.toUpper();
}

QWidget* AddressDelegate::createEditor ( QWidget*, const QStyleOptionViewItem&, const QModelIndex& ) const
{
    return 0;
}
