#ifndef TREEWIDGETITEM_H
#define TREEWIDGETITEM_H

#include<QTreeWidgetItem>
class TreeWidgetItem : public QTreeWidgetItem
{
public:
    TreeWidgetItem(TreeWidgetItem *parent, int type = Type);
    explicit TreeWidgetItem(QTreeWidget *view, int type = Type);
    void setPre(TreeWidgetItem* item);
    void setNext(TreeWidgetItem* item);
    TreeWidgetItem* getPre();
    TreeWidgetItem* getNext();
    QString getPath();
private:
    TreeWidgetItem* _pre=nullptr;
    TreeWidgetItem* _next=nullptr;
};

#endif // TREEWIDGETITEM_H
