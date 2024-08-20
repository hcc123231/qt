#include "picshowwind.h"
#include "ui_picshowwind.h"
#include<QTimer>
#include<QFileInfo>
#include<QPixmap>
#include<QFile>
#include<QVector>
#include<QDebug>
#define PIX_COUNTS 100
PicShowWind::PicShowWind(QMediaPlayer* mus_player,TreeWidgetItem* root_item,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PicShowWind)
{
    ui->setupUi(this);
    _mus_player=mus_player;
    if(root_item==nullptr){
        return;
    }
    _root_item=root_item;
    QVector<QPixmap> vector;


    int item_count=_root_item->childCount();
    for(int i=0;i<item_count;i++){
        QString path=dynamic_cast<TreeWidgetItem*>(root_item->child(i))->getPath();
        QFileInfo fileinfo(path);
        if(fileinfo.isDir()){
            continue;
        }
        QFile file(path);
        if(!file.open(QFile::ReadOnly)){
            qDebug()<<"failed to open";
            continue;
        }
        QByteArray byte=file.readAll();
        file.close();
        QPixmap pix;
        pix.loadFromData(byte);
        vector.push_back(pix);
    }
    if(vector.size()==0||vector.size()==1){
        return;
    }
    ui->widget->getVec(vector);
    this->showMaximized();
    if(mus_player){
        _mus_player->play();
    }

}



PicShowWind::~PicShowWind()
{
    delete ui;
}

void PicShowWind::closeEvent(QCloseEvent *event)
{
    _mus_player->pause();
    delete this;
}

