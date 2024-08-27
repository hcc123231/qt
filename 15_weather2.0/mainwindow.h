#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include"weather_data.h"
#include <QMainWindow>
#include<QNetworkAccessManager>
#include<QVector>
#include<QLabel>
#include<QList>
#include<QMap>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void getWeatherInfo(QString code);
    void parseJson(QByteArray date);
    void pushLabel();
    void setColor(QVector<Dataset>vec,QList<QLabel*>list);
    void setIcon(QVector<Dataset>vec,QList<QLabel*>list);
protected:
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
    virtual void paintEvent(QPaintEvent *event);
private:
    Ui::MainWindow *ui;
    QPoint relative_point;
    QNetworkAccessManager* net_acce_manege;
    QVector<Dataset>data_vec;

    QList<QLabel*>week_list;
    QList<QLabel*>date_list;
    QList<QLabel*>icon_list;
    QList<QLabel*>wea_list;
    QList<QLabel*>quality_list;
    QList<QLabel*>fx_list;
    QList<QLabel*>fl_list;
    int high_ave;
    int low_ave;
    QMap<QString,QString>map;
    bool isUp=false;
    int m_first_height;
    int m_second_height;
    int m_last_height;
    qint64 last_time=0;
    QString m_city_name;
public slots:
    void slotClose();
    void slotHandleReply(QNetworkReply* reply);
    void slotUpdate();
    void slotTimeUpdate();
signals:
    void sigPaint(QList<QLabel*>,int hi,int lo,QVector<Dataset>);
};

#endif // MAINWINDOW_H
