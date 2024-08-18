#ifndef DISPALYPICTHREAD_H
#define DISPALYPICTHREAD_H
#include<QThread>
#include<QTreeWidget>
#include<QTreeWidgetItem>
#include"mainwindow.h"
class DispalyPicThread : public QThread
{
public:
    DispalyPicThread(QObject *parent,QTreeWidget* tree_widget,
                     MainWindow* mainwindow);
    void dispaly(QTreeWidget* tree_widget,MainWindow* mainwindow
                 ,QTreeWidgetItem* item,int column=0);
protected:
    virtual void run();
private:
    QTreeWidget* _tree_widget;
    MainWindow* _mainwindow;
    QTreeWidgetItem* _item;
};

#endif // DISPALYPICTHREAD_H
