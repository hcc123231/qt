#include "importthread.h"
#include<QDir>
#include<QFile>
#include<QDebug>
ImportThread::ImportThread(QString top_path,TreeWidgetItem* root_item,QString tar_path,
                           QString operate_path,
                           QObject *parent):
    QThread (parent)
{
    _top_path=top_path;
    _root_item=root_item;
    _tar_path=tar_path;
    _operate_path=operate_path;
}

void ImportThread::importfiles(QString tar_path,QString operate_path)
{
    //先将用户选择的文件夹添加到目标文件夹下

    QFile temp_file;
    QFileInfo info=operate_path;
    QDir tar_dir(tar_path);
    tar_dir.mkpath(info.fileName());
    //再将文件夹下的文件添加到目标文件夹下
    QDir selected_dir(operate_path);
    QStringList file_list=selected_dir.entryList
            (QDir::Dirs|QDir::Files|QDir::NoDotAndDotDot);
    for(int i=0;i<file_list.size();i++){
        //先变成绝对路径
        QString abs_path=selected_dir.absoluteFilePath(file_list[i]);
        //对文件夹做处理，直接添加到指定目录下
        QFileInfo info2=abs_path;
        if(info2.isDir()){
            QString tar_str=tar_path+QDir::separator()+info.fileName();
            importfiles(tar_str,abs_path);
            continue;
        }

        //对文件处理，通过copy和open操作
        QDir dir_t(tar_path+QDir::separator()+info.fileName());
        QFileInfo info1=abs_path;
        QString file_path=dir_t.absoluteFilePath(info1.fileName());
        QFile file(file_path);
        QFileInfo check_info(abs_path);
        if(check_info.suffix()!="png"&&check_info.suffix()!="jpg"&&check_info.suffix()
                !="jpeg"){
            continue;
        }
        if(!QFile::copy(abs_path,file_path)){
            qDebug("failed to copy");
        }
    }

}

void ImportThread::loadItems(QString path, TreeWidgetItem *root_item)
{
    if(path==""||root_item==nullptr){
        qDebug("param is null");
        return;
    }
    root_item->setPre(nullptr);
    root_item->setNext(nullptr);
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
        TreeWidgetItem* son_item=new TreeWidgetItem(root_item);
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
            loadItems(abs_path,son_item);
        }

    }

}

void ImportThread::run()
{
    importfiles(_tar_path,_operate_path);
    loadItems(_top_path,_root_item);
}
