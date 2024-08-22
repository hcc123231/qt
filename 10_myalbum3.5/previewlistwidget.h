#ifndef PREVIEWLISTWIDGET_H
#define PREVIEWLISTWIDGET_H
#include<QPixmap>
#include<QListWidget>
#include<QVector>
#include"previewlistwidgetitem.h"
class PreViewListWidget : public QListWidget
{
public:
    explicit PreViewListWidget(QWidget *parent = nullptr);
    void getPic(const QPixmap* pixmap,int cnt);
    void setChange(bool change);
public slots:
    void slotCircleSelected();
    void slotCircleChange();
private:
    QVector<PreViewListWidgetItem*>_items_vec;
    bool _change=false;
    int _idx=0;
};

#endif // PREVIEWLISTWIDGET_H
