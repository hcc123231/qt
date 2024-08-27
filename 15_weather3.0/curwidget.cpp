#include "curwidget.h"
#include<QPainter>
#include<QTimer>
#include<QDebug>
#include<QGraphicsOpacityEffect>
CurWidget::CurWidget(QWidget *parent) : QWidget(parent)
{
    this->setAttribute(Qt::WA_StyledBackground, true);
    QGraphicsOpacityEffect* effect=new QGraphicsOpacityEffect(this);
    effect->setOpacity(1.0);
    this->setGraphicsEffect(effect);
    QTimer* time=new QTimer(this);
    time->start(30);
    connect(time,&QTimer::timeout,this,[=](){
        update();
    });
}

void CurWidget::paintEvent(QPaintEvent *event)
{
    if (m_list.isEmpty()) {
    }
    else{
    //this->setStyleSheet("QWidget{background-color: rgb(202, 202, 202);");
    QPainter painter(this);
    QPen pen;
    for (int i = 0; i < 6; ++i) {
            //高温曲线点
        /*(255, 170, 0)
        background-color: rgb(255, 235, 84);*/
        pen.setColor(QColor(255, 235, 84));
        painter.setPen(pen);
        QBrush brush1(QColor(255, 235, 84));
        painter.setBrush(brush1);
            if(m_data_vec[i].m_high>m_high_ave){

                m_h=30+(m_data_vec[i].m_high-m_high_ave)*20;
            }
            else{

                m_h=30-(m_data_vec[i].m_high-m_high_ave)*20;
            }
            QPoint center(m_list[i]->pos().x()+m_list[i]->width()/2, m_h);
            painter.drawEllipse(center, 5, 5);
            QFont font;
            font.setBold(true);
            font.setPointSize(10);
            painter.setFont(font);

            QPoint textp(m_list[i]->pos().x()+m_list[i]->width()/2-5,m_h-5);
            painter.drawText(textp,QString("%1°C").arg(m_data_vec[i].m_high));
            m_high_arr[i]=QPoint(m_list[i]->pos().x()+m_list[i]->width()/2, m_h);

            //低温曲线点
            //QColor(5, 255, 247)
            pen.setColor(QColor(176, 255, 253));
            painter.setPen(pen);
            QBrush brush(QColor(5, 255, 247));

            painter.setBrush(brush);
            if(m_data_vec[i].m_low>m_low_ave){

                m_h=200+(m_data_vec[i].m_low-m_low_ave)*20;
            }
            else{

                m_h=200-(m_data_vec[i].m_low-m_low_ave)*20;
            }
            QPoint center2(m_list[i]->pos().x()+m_list[i]->width()/2, m_h);
            painter.drawEllipse(center2, 5, 5);
            QPoint textp2(m_list[i]->pos().x()+m_list[i]->width()/2-5,m_h-5);
            painter.drawText(textp2,QString("%1°C").arg(m_data_vec[i].m_low));
            m_low_arr[i]=QPoint(m_list[i]->pos().x()+m_list[i]->width()/2, m_h);

    }
    //绘制曲线
    for(int i=0;i<5;i++){
        pen.setColor(QColor(255, 235, 84));
        pen.setWidth(5);
        painter.setPen(pen);
        painter.drawLine(m_high_arr[i],m_high_arr[i+1]);
        /*background-color: rgb(176, 255, 253);
        QColor(5, 255, 247)*/
        pen.setColor(QColor(176, 255, 253));
        pen.setWidth(5);
        painter.setPen(pen);
        painter.drawLine(m_low_arr[i],m_low_arr[i+1]);
    }

    }
}

void CurWidget::slotPaint(QList<QLabel *> list,int high_ave,int low_ave,QVector<Dataset>data_vec)
{

    m_list=list;
    m_high_ave=high_ave;
    m_low_ave=low_ave;
    m_data_vec=data_vec;
}
