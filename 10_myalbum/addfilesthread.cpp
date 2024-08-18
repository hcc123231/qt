#include "addfilesthread.h"
#include<QDebug>
AddFilesThread::AddFilesThread()
{

}

AddFilesThread::AddFilesThread(QObject *parent,QString path,QTreeWidgetItem* top_item):
    QThread (parent),_path(path),_top_item(top_item)
{

}

void AddFilesThread::addFiles(QString path, QTreeWidgetItem *top_item)
{
    QDir top_dir(path);
    //得到的是相对路径
    QStringList files_list=top_dir.entryList(QDir::Dirs|QDir::Files|QDir::NoDotAndDotDot);

    for(int i=0;i<files_list.size();i++)
    {
        QString abs_pa=top_dir.absoluteFilePath(files_list.at(i));
        QFileInfo info(abs_pa);
        if(info.isFile()&&info.suffix()!="jpg"&&info.suffix()!="jpeg"&&info.suffix()!="png")
        {
            continue;
        }
        QTreeWidgetItem* son_item=new QTreeWidgetItem(top_item);
        QString abs_path=top_dir.absoluteFilePath(files_list.at(i));
        QFileInfo fileinfo(abs_path);
        son_item->setData(0,Qt::DisplayRole,fileinfo.fileName());
        son_item->setToolTip(0,abs_path);
        if(fileinfo.isDir())
        {
            addFiles(abs_path,son_item);
        }
    }
}

AddFilesThread::~AddFilesThread()
{

}
void AddFilesThread::run()
{
    addFiles(_path,_top_item);
}

