#ifndef FILETHREAD_H
#define FILETHREAD_H
#include<QThread>
#include<QTreeWidgetItem>
#include<QDir>
class FileThread : public QThread
{
public:
    FileThread(QObject *parent,QString path,QTreeWidgetItem* item);
    void addFile(QString path,QTreeWidgetItem* item);
protected:
    virtual void run();
private:
    QString _path;
    QTreeWidgetItem* _item;
};

#endif // FILETHREAD_H
