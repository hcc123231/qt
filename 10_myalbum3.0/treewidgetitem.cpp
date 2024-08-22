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

TreeWidgetItem *TreeWidgetItem::getPre()
{
    return _pre;
}

TreeWidgetItem *TreeWidgetItem::getNext()
{
    return _next;
}

QString TreeWidgetItem::getPath()
{
    QVariant var=this->toolTip(0);
    QString s=var.toString();
    return s;
}
