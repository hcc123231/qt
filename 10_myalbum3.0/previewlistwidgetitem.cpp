#include "previewlistwidgetitem.h"


PreViewListWidgetItem::PreViewListWidgetItem(const QIcon &icon,
                                             const QString &text,
                                             QListWidget *view, int type):
    QListWidgetItem (icon,text,view,type)
{

}
