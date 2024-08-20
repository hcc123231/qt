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
signals:
protected:
    virtual void paintEvent(QPaintEvent *event);
    virtual void closeEvent(QCloseEvent *event);
public slots:
private:
    double _factor;
    QPixmap _cur_pixmap;
    QPixmap _nex_pixmap;
    QVector<QPixmap> vector_pix;
    int cnt;
    int count;
};

#endif // SLIDESHOW_H
