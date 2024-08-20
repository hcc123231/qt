#include "filethread.h"
#include<QDebug>
FileThread::FileThread(QObject *parent, QString path,TreeWidgetItem* item\
                       ,QTreeWidget* tree_widget):
    QThread (parent),_path(path),_item(item),_tree_widget(tree_widget)
{
    _preItem=nullptr;
    _nextItem=nullptr;
}

void FileThread::addFile(QString path,TreeWidgetItem *item)
{
    item->setPre(nullptr);
    item->setNext(nullptr);
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
        TreeWidgetItem* son_item=new TreeWidgetItem(item);
        son_item->setPre(nullptr);
        son_item->setNext(nullptr);

        son_item->setPre(_preItem);

        if(i>0){
            _preItem->setNext(son_item);
        }
        _preItem=son_item;
        QString abs_path=top_dir.absoluteFilePath(files_list.at(i));
        QFileInfo fileinfo(abs_path);
        son_item->setData(0,Qt::DisplayRole,fileinfo.fileName());
        son_item->setData(0,Qt::DecorationRole,QIcon(":/97ceada96bdf445fbf955814b2b703aa.png"));
        son_item->setToolTip(0,abs_path);
        if(fileinfo.isDir())
        {
            son_item->setData(0,Qt::DecorationRole,QIcon(":/6a9c6d12bd6c4a53b27e4cd35dfc8b3b.png"));
            addFile(abs_path,son_item);
        }

    }

}

void FileThread::run()
{
    addFile(_path,_item);
}
