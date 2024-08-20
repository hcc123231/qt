#include "slideshow.h"
#include<QTimer>
#include<QPainter>
#include<QDebug>
SlideShow::SlideShow(QWidget *parent) : QWidget(parent)
{
    _factor=0.0;
    cnt=0;
    QTimer* timer=new QTimer(this);
    timer->start(30);
    connect(timer,&QTimer::timeout,this,[=](){
        update();
        if(_factor>=1){
            if(cnt>=count-1){
                cnt=0;
            }
            if(count-1>cnt){
                this->setTwoPic(vector_pix[cnt],vector_pix[cnt+1]);
            }

            cnt++;
            _factor=0.0;
        }
        else _factor+=0.01;
    });
}

void SlideShow::setTwoPic(QPixmap cur_pixmap, QPixmap nex_pixmap)
{

   if(cur_pixmap.isNull()||nex_pixmap.isNull()){
        qDebug()<<"settwopic not recv anything";
    }
    _cur_pixmap=cur_pixmap;
    _nex_pixmap=nex_pixmap;
}

void SlideShow::getVec(QVector<QPixmap> v)
{
    vector_pix=v;
    count=vector_pix.size();
    if(count-1>cnt){
        this->setTwoPic(vector_pix[cnt],vector_pix[cnt+1]);
    }
    cnt++;

}


void SlideShow::paintEvent(QPaintEvent *event)
{
    if(_cur_pixmap.isNull()){
        qDebug()<<"pixmap is null";
    }
    if(_nex_pixmap.isNull()){
        qDebug()<<"pixmap2 is null";
    }

    QPainter painter(this);

    QPixmap cur_pixmap(_cur_pixmap);
    QPixmap nex_pixmap(_nex_pixmap);


    QRect rect=geometry();
    int x=rect.width();
    int y=rect.height();
    cur_pixmap= cur_pixmap.scaled(x,y,Qt::KeepAspectRatio);
    QPixmap cur_pix_device(cur_pixmap.size());
    cur_pix_device.fill(Qt::transparent);
    QPainter cur_pixmap_painter(&cur_pix_device);
    cur_pixmap_painter.setCompositionMode(QPainter::CompositionMode_Source);
    cur_pixmap_painter.drawPixmap(0,0,cur_pixmap);
    cur_pixmap_painter.setCompositionMode(QPainter::CompositionMode_DestinationIn);
    int alpha=static_cast<int>(255*(1-_factor));
    cur_pixmap_painter.fillRect(this->rect(),QColor(0,0,0,alpha));
    int start_x=(x-cur_pixmap.width())/2;
    int start_y=(y-cur_pixmap.height())/2;
    painter.drawPixmap(start_x,start_y,cur_pix_device);
    cur_pixmap_painter.end();

    nex_pixmap=nex_pixmap.scaled(x,y,Qt::KeepAspectRatio);
    QPixmap nex_pix_device(nex_pixmap.size());
    nex_pix_device.fill(Qt::transparent);
    QPainter nex_pixmap_painter(&nex_pix_device);
    nex_pixmap_painter.setCompositionMode(QPainter::CompositionMode_Source);
    nex_pixmap_painter.drawPixmap(0,0,nex_pixmap);
    nex_pixmap_painter.setCompositionMode(QPainter::CompositionMode_DestinationIn);
    alpha=static_cast<int>(255*_factor);
    nex_pixmap_painter.fillRect(this->rect(),QColor(0,0,0,alpha));
    painter.drawPixmap(start_x,start_y,nex_pix_device);
    nex_pixmap_painter.end();
}

void SlideShow::closeEvent(QCloseEvent *event)
{
    delete this;
}
