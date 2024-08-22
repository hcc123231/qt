#include "previewlistwidget.h"
#include<QLabel>
#include<QHBoxLayout>
#include<QIcon>
#include"previewlistwidgetitem.h"
#include<QPixmap>
#include<QPainter>
#include<QDebug>
#include<QObject>
#include<QTimer>
PreViewListWidget::PreViewListWidget(QWidget *parent):
    QListWidget (parent)
{
    this->setFlow(QListView::LeftToRight);
    this->setViewMode(QListView::IconMode);
    this->setFlow(QListView::LeftToRight);
    this->setIconSize(QSize(90+10,90+10));
    this->setSpacing(5);

}

void PreViewListWidget::getPic(const QPixmap *pixmap,int cnt)
{
    if(pixmap==nullptr){
        return;
    }
    QPixmap src_pix=*pixmap;
    QPixmap dist_pix(90+10,90+10);
    src_pix=src_pix.scaled(95+10,90+10,Qt::KeepAspectRatio);
    dist_pix.fill(QColor(220,220,220,50));
    QPainter painter(&dist_pix);
    int x=(dist_pix.width()-src_pix.width())/2;
    int y=(dist_pix.height()-src_pix.height())/2;
    painter.drawPixmap(x,y,src_pix);
    QIcon icon(dist_pix);
    PreViewListWidgetItem* item=new PreViewListWidgetItem(icon,"",this);

    item->setSizeHint(QSize(95+10,90+10));
    this->addItem(item);
    _items_vec.push_back(item);
    item->setSelected(true);
    item->setData(Qt::UserRole, QVariant(QString("%1").arg(cnt)));
}

void PreViewListWidget::setChange(bool change)
{
    _change=change;
}

void PreViewListWidget::slotCircleSelected()
{
    _change=true;
    if(!_items_vec.size()){
        return;
    }
    _items_vec[_idx]->setSelected(true);
    _idx++;

}

void PreViewListWidget::slotCircleChange()
{
    if(!_change){
        return;
    }
    if(_idx>=_items_vec.size()){
        _idx=0;
        return;
    }
    _items_vec[_idx]->setSelected(true);

    _idx++;
    if(_idx>=_items_vec.size()){
        _idx=0;
        _change=false;
    }
}

