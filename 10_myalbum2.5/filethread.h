#ifndef FILETHREAD_H
#define FILETHREAD_H
#include<QThread>
#include<QTreeWidgetItem>
#include<QDir>
#include"treewidgetitem.h"
#include<QTreeWidget>
class FileThread : public QThread
{
public:
    FileThread(QObject *parent,QString path,TreeWidgetItem* item,QTreeWidget* tree_widget);
    void addFile(QString path,TreeWidgetItem* item);
protected:
    virtual void run();
private:
    QTreeWidget* _tree_widget;
    QString _path;
    TreeWidgetItem* _nextItem;
    TreeWidgetItem* _preItem;
    TreeWidgetItem* _item;
};

#endif // FILETHREAD_H
