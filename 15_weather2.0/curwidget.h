#ifndef CURWIDGET_H
#define CURWIDGET_H
#include<QWidget>
#include<QLabel>
#include<QList>
#include"weather_data.h"
class CurWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CurWidget(QWidget *parent = nullptr);
protected:
    virtual void paintEvent(QPaintEvent *event);
signals:

public slots:
    void slotPaint(QList<QLabel*>list,int high_ave,int low_ave,QVector<Dataset>data_vec);
private:
    QList<QLabel*> m_list;
    int m_high_ave;
    int m_low_ave;
    QVector<Dataset> m_data_vec;
    int m_h;
    QPoint m_high_arr[6];
    QPoint m_low_arr[6];
};

#endif // CURWIDGET_H
