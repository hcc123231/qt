#include "widget.h"
#include<QDebug>
Widget::Widget(QWidget *parent) : QWidget(parent)
{

}

void Widget::setBUtton(QToolButton *pre_btn, QToolButton *next_btn)
{
    _pre_btn=pre_btn;
    _next_btn=next_btn;
}

void Widget::enterEvent(QEvent *event)
{
    if(_pre_btn){
        _pre_btn->show();
    }
    if(_next_btn){
        _next_btn->show();
    }

}

void Widget::leaveEvent(QEvent *event)
{
    if(_pre_btn){
        _pre_btn->hide();
    }
    if(_next_btn){
        _next_btn->hide();
    }

}


