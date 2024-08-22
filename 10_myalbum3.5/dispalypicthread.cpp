#include "dispalypicthread.h"
#include"ui_mainwindow.h"
#include<QFile>
#include<QDebug>
#include<QPixmap>

DispalyPicThread::DispalyPicThread(QObject *parent, QTreeWidget *tree_widget
                                   ,MainWindow* mainwindow):
    QThread (parent),_tree_widget(tree_widget),_mainwindow(mainwindow)
{

}

void DispalyPicThread::dispaly(QTreeWidget *tree_widget,MainWindow* mainwindow
                               ,TreeWidgetItem* item,int column)
{
    QVariant vari=item->toolTip(column);
    if(vari.isNull())
    {
        qDebug()<<"vari is null";
        return;
    }
    QString file_path=vari.toString();
    QFile pic_file(file_path);
    if(!pic_file.open(QFile::ReadOnly)){
        qDebug()<<"failed to open file";
        return;
    }
    QByteArray file_byte=pic_file.readAll();
    pic_file.close();
    QPixmap pix;
    pix.loadFromData(file_byte);
    mainwindow->getui()->dispaly_lab->setPixmap(pix);
    mainwindow->getui()->dispaly_lab->setFixedSize(1800,1100);
    mainwindow->getui()->dispaly_lab->setScaledContents(true);


}

void DispalyPicThread::run()
{
    dispaly(_tree_widget,_mainwindow,_item);
}
