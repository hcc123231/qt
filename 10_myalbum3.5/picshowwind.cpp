#include "picshowwind.h"
#include "ui_picshowwind.h"
#include<QTimer>
#include<QFileInfo>
#include<QPixmap>
#include<QFile>
#include<QVector>
#include<QDebug>
#include"slideshow.h"
#include<QMenu>
#define PIX_COUNTS 100
PicShowWind::PicShowWind(QMediaPlayer* mus_player,TreeWidgetItem* root_item,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PicShowWind)
{
    ui->setupUi(this);
    /*将其中的widget和listWidget管理起来，如果widget发出circle信号，那么listWidget需要将
    vector中的第一个item设置为selected状态*/
    connect(ui->widget,&SlideShow::sigCircle,this,[=](){
        ui->listWidget->slotCircleSelected();
    });
    connect(ui->widget,&SlideShow::sigSwitchPic,this,[=](){
        ui->listWidget->slotCircleChange();
    });
    this->setWindowFlag(Qt::FramelessWindowHint);
    QMenu* speed_menu=new QMenu(ui->top_widget);
    QAction* a1=new QAction(this);
    QAction* a2=new QAction(this);
    QAction* a3=new QAction(this);
    QAction* a4=new QAction(this);
    QAction* a5=new QAction(this);
    QAction* a6=new QAction(this);
    a1->setText("x1.0");
    a2->setText("x1.25");
    a3->setText("x1.5");
    a4->setText("x2.0");
    a5->setText("x5.0");
    a6->setText("x10.0");
    speed_menu->addAction(a1);
    speed_menu->addAction(a2);
    speed_menu->addAction(a3);
    speed_menu->addAction(a4);
    speed_menu->addAction(a5);
    speed_menu->addAction(a6);
    speed_menu->setStyleSheet("QMenu{border:2px solid white;font-size:16pt}");
    connect(this,&PicShowWind::sigAdjustSPeed,ui->widget,&SlideShow::slotRecvSpeed);
    connect(a1,&QAction::triggered,this,[=](){
        emit this->sigAdjustSPeed(1.0);
        emit ui->widget->sigSpeedChange();
    });
    connect(a2,&QAction::triggered,this,[=](){
        emit this->sigAdjustSPeed(1.25);
        emit ui->widget->sigSpeedChange();
    });
    connect(a3,&QAction::triggered,this,[=](){
        emit this->sigAdjustSPeed(1.5);
        emit ui->widget->sigSpeedChange();
    });
    connect(a4,&QAction::triggered,this,[=](){
        emit this->sigAdjustSPeed(2.0);
        emit ui->widget->sigSpeedChange();
    });
    connect(a5,&QAction::triggered,this,[=](){
        emit this->sigAdjustSPeed(5.0);
        emit ui->widget->sigSpeedChange();
    });
    connect(a6,&QAction::triggered,this,[=](){
        emit this->sigAdjustSPeed(10.0);
        emit ui->widget->sigSpeedChange();
    });

    QToolButton* speed_btn=new QToolButton(ui->top_widget);
    speed_btn->setObjectName("speed_btn");
    speed_btn->setIcon(QIcon(":/a490c5ef59684e918fb1705b1dc1ce83.png"));
    speed_btn->setIconSize(QSize(100,100));
    speed_btn->setMenu(speed_menu);
    connect(speed_btn,&QToolButton::clicked,this,[=](){
        if(speed_btn->menu()){
            speed_btn->showMenu();
        }
    });
    ui->horizontalLayout->addWidget(speed_btn);




    ui->stop_btn->setIconSize(QSize(100,100));
    ui->close_btn->setIconSize(QSize(100,100));

    ui->stop_btn->setIcon(QIcon(":/672e99dc569a4bd397bf6ef38624f59c.png"));

    ui->close_btn->setIcon(QIcon(":/c9645dcdfeef4c2692d2f1ab71b50295.png"));
    connect(ui->close_btn,&QToolButton::clicked,this,[=](){
        this->close();
    });
    connect(ui->stop_btn,&QToolButton::clicked,this,[=](){
        if(isStop){
            ui->widget->getTimer()->start();
            ui->stop_btn->setIcon(QIcon(":/672e99dc569a4bd397bf6ef38624f59c.png"));
            _mus_player->play();
            isStop=false;

        }
        else{
            ui->widget->getTimer()->stop();
            ui->stop_btn->setIcon(QIcon(":/e0a07cd80e09495ea59e715ad48eee7a.png"));
            _mus_player->pause();
            isStop=true;
        }

    });
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
    if(_pic_idx>=0&&_pic_idx<vector.size()){
        ui->listWidget->getPic(&vector[_pic_idx],_pic_idx);
        _pic_idx++;
    }

    QTimer* time=new QTimer(this);
    time->start(1000);
    time->setSingleShot(true);
    connect(time,&QTimer::timeout,this,[=](){
        if(_pic_idx>=vector.size()){
            return;
        }
        ui->listWidget->getPic(&vector[_pic_idx],_pic_idx);
        _pic_idx++;
    });

    connect(ui->widget,&SlideShow::sigSwitchPic,this,[=](){
        if(_pic_idx<0||_pic_idx>=vector.size()){
            return;
        }
        ui->listWidget->getPic(&vector[_pic_idx],_pic_idx);

        _pic_idx++;
    });

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
    if(_mus_player){
        _mus_player->stop();
    }

    delete this;
}

