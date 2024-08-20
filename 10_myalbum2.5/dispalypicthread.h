#ifndef DISPALYPICTHREAD_H
#define DISPALYPICTHREAD_H
#include<QThread>
#include<QTreeWidget>
#include<QTreeWidgetItem>
#include"mainwindow.h"
#include"treewidgetitem.h"
class DispalyPicThread : public QThread
{
public:
    DispalyPicThread(QObject *parent,QTreeWidget* tree_widget,
                     MainWindow* mainwindow);
    void dispaly(QTreeWidget* tree_widget,MainWindow* mainwindow
                 ,TreeWidgetItem* item,int column=0);
protected:
    virtual void run();
private:
    QTreeWidget* _tree_widget;
    MainWindow* _mainwindow;
    TreeWidgetItem* _item;
};

#endif // DISPALYPICTHREAD_H
