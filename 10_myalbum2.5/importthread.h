#ifndef IMPORTTHREAD_H
#define IMPORTTHREAD_H
#include"treewidgetitem.h"
#include<QThread>
class ImportThread : public QThread
{
public:
    explicit ImportThread(QString top_path,TreeWidgetItem* root_item,QString tar_path,
                          QString operate_path,
                          QObject *parent = nullptr);
    void importfiles(QString tar_path,QString operate_path);
    void loadItems(QString path,TreeWidgetItem *root_item);
protected:
    virtual void run();
private:
    QString _tar_path;
    QString _operate_path;
    TreeWidgetItem* _root_item;
    TreeWidgetItem* _preItem;
    QString _top_path;
};

#endif // IMPORTTHREAD_H
