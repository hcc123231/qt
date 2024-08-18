#include "treewidgetitem.h"

TreeWidgetItem::TreeWidgetItem(TreeWidgetItem *parent, int type):
    QTreeWidgetItem (parent)
{

}

TreeWidgetItem::TreeWidgetItem(QTreeWidget *view, int type):
    QTreeWidgetItem (view)
{

}

void TreeWidgetItem::setPre(TreeWidgetItem *item)
{
    _pre=item;
}

void TreeWidgetItem::setNext(TreeWidgetItem *item)
{
    _next=item;
}
