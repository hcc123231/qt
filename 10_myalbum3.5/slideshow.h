#ifndef SLIDESHOW_H
#define SLIDESHOW_H
#include<QWidget>
#include<QVector>
class SlideShow : public QWidget
{
    Q_OBJECT
public:
    explicit SlideShow(QWidget *parent = nullptr);
    void setTwoPic(QPixmap cur_pixmap,QPixmap nex_pixmap);
    void getVec(QVector<QPixmap>v);
    QTimer* getTimer();
signals:
    void sigSwitchPic();
    void sigSpeedChange();
    void sigCircle();
protected:
    virtual void paintEvent(QPaintEvent *event);
    virtual void closeEvent(QCloseEvent *event);
public slots:
    void slotRecvSpeed(double speed);

private:
    double _factor;
    QPixmap _cur_pixmap;
    QPixmap _nex_pixmap;
    QVector<QPixmap> vector_pix;
    int cnt;
    int count;
    QTimer* _timer=nullptr;
    double _speed=1.0;
    double _update_time=30;

};

#endif // SLIDESHOW_H
