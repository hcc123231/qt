#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QToolButton>
#include<QMouseEvent>
#include<QPoint>
#include<QPropertyAnimation>
#include<QAnimationGroup>
#include<QNetworkReply>
#include<QMessageBox>
#include<QJsonDocument>
#include<QJsonObject>
#include<QJsonArray>
#include<QJsonParseError>
#include<QJsonValue>
#include<QSplitter>
#include<QSequentialAnimationGroup>
#include<QTimer>
#include<QPainter>
#include<QFile>
#include<QChar>
#include<QBrush>
#include<cstring>
#include<QByteArray>
#include<QDateTime>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/weather.png"));
    connect(ui->mini_btn,&QToolButton::clicked,this,[=](){
        this->showMinimized();
    });
    QPixmap kk(":/mini2.png");
    kk.scaled(50,50,Qt::KeepAspectRatio);
    ui->mini_btn->setIcon(QIcon(kk));
    ui->mini_btn->setStyleSheet("QToolButton{border:0px}");
    m_first_height=ui->first_wid->height();
    m_second_height=ui->weainfo_wid->height();
    m_last_height=ui->last_wid->height();
    ui->lineEdit->setStyleSheet("QLineEdit{border:2px solid black;border-radius:20px}");
    QPixmap p(":/close.png");
    p=p.scaled(60,60,Qt::KeepAspectRatio);
    QIcon icon(p);
    ui->close_btn->setIcon(icon);
    ui->close_btn->setStyleSheet("QToolButton{border:0px}");
    pushLabel();
    //设置为固定大小
    //this->setFixedSize(600,712);
    //将边框去掉，由于设置了无边框，所以用户无法通过鼠标拉升窗口
    this->setWindowFlag(Qt::FramelessWindowHint);
    //关闭按钮的信号与槽
    connect(ui->close_btn,&QToolButton::clicked,this,&MainWindow::slotClose);
    net_acce_manege=new QNetworkAccessManager();

    //从文件中读出城市名与城市编码
    QFile file("://citycode (1).json");
    if(!file.open(QFile::ReadOnly)){
        qDebug()<<"文件读取失败";
        return;
    }
    QByteArray byte=file.readAll();
    file.close();
    QJsonParseError e;
    QJsonDocument d=QJsonDocument::fromJson(byte,&e);
    if(e.error!=QJsonParseError::NoError){
        return;
    }
    QJsonArray root_arr=d.array();
    for(int i=0;i<root_arr.size();i++){
        QJsonObject object=root_arr[i].toObject();
        map.insert(object.value("city_name").toString(),object.value("city_code").toString());
    }
    connect(net_acce_manege,&QNetworkAccessManager::finished,this,&MainWindow::slotHandleReply);
    getWeatherInfo(map["怀化"]);
    //m_city_name="怀化";
    //QTimer* update=new QTimer(this);
    //update->start(10000);
    //connect(update,&QTimer::timeout,this,&MainWindow::slotTimeUpdate);


    connect(ui->lineEdit,&QLineEdit::returnPressed,this,&MainWindow::slotUpdate);

}

MainWindow::~MainWindow()
{
    delete ui;
}
//申请数据
void MainWindow::getWeatherInfo(QString code)
{
    QString city_code=code;
    QUrl url(QString("http://t.weather.itboy.net/api/weather/city/%1").arg(city_code));
    QNetworkRequest request(url);
    net_acce_manege->get(request);

}
//解析数据并且更新到ui界面上
void MainWindow::parseJson(QByteArray date)
{
    data_vec.clear();
    QJsonParseError error;
    QJsonDocument doc=QJsonDocument::fromJson(date,&error);
    if(error.error!=QJsonParseError::NoError){
        return;
    }
    QJsonObject root_obj=doc.object();

    QString t_date=root_obj.value("date").toString();
    ui->date_lab->setAlignment(Qt::AlignCenter);
    ui->date_lab->setText(QDateTime::fromString(t_date,"yyyymmdd").toString("yyyy/mm/dd"));
    ui->date_lab->setStyleSheet("QLabel{font-size:13pt}");

    QJsonObject data_obj=root_obj.value("data").toObject();
    QJsonArray forecast_arr=data_obj.value("forecast").toArray();
    for(int i=0;i<6;i++){
        QJsonObject obj=forecast_arr[i].toObject();


        Dataset data_set=Dataset((obj.value("high").toString().remove(-1,1)).remove(0,2).toInt(),(obj.value("low").toString().remove(-1,1)).remove(0,2).toInt(),
                obj.value("ymd").toString(),obj.value("week").toString(),
                obj.value("aqi").toInt(),obj.value("fx").toString(),
                obj.value("fl").toString(),obj.value("type").toString());

        data_vec.push_back(data_set);
    }
    ui->week_lab->setAlignment(Qt::AlignCenter);
    ui->week_lab->setText(data_vec[0].m_week);
    ui->week_lab->setStyleSheet("QLabel{font-size:14pt}");
    ui->area_lab->setAlignment(Qt::AlignCenter);
    ui->area_lab->setText(root_obj.value("cityInfo").toObject().value("city").toString());
    ui->area_lab->setStyleSheet("QLabel{font-size:16pt}");
    ui->tip_lab->setText("感冒指数："+data_obj.value("ganmao").toString());
    ui->label->setAlignment(Qt::AlignCenter);
    ui->label->setText(data_vec[0].m_type);
    ui->label->setStyleSheet("QLabel{font-size:12pt}");
    ui->temp_lab->setAlignment(Qt::AlignCenter);
    ui->temp_lab->setText(data_obj.value("wendu").toString()+"C°");
    ui->temp_lab->setStyleSheet("QLabel{font-size:16pt}");
    ui->temp_range_lab->setAlignment(Qt::AlignCenter);
    ui->temp_range_lab->setText(QString("%1°C").arg(data_vec[0].m_low)+"~"+QString("%1°C").arg(data_vec[0].m_high));
    ui->fx_lab->setAlignment(Qt::AlignCenter);
    ui->fx_lab->setText(data_vec[0].m_fx);
    ui->fl_lab->setAlignment(Qt::AlignCenter);
    ui->fl_lab->setText(data_vec[0].m_fl);
    ui->pm_lab->setAlignment(Qt::AlignCenter);
    ui->pm_lab->setText("PM2.5");
    int pm_index=data_obj.value("pm25").toInt();
    ui->pm_index_lab->setAlignment(Qt::AlignCenter);
    ui->pm_index_lab->setText(QString("%1").arg(pm_index));
    ui->sd_lab->setAlignment(Qt::AlignCenter);
    ui->sd_lab->setText("湿度");
    ui->sd_index_lab->setAlignment(Qt::AlignCenter);
    ui->sd_index_lab->setText(data_obj.value("shidu").toString());
    ui->zl_lab->setAlignment(Qt::AlignCenter);
    ui->zl_lab->setText("空气质量");
    ui->zl_index_lab->setAlignment(Qt::AlignCenter);
    ui->zl_index_lab->setText(data_obj.value("quality").toString());
    ui->week1->setAlignment(Qt::AlignCenter);
    ui->week1->setText("今天");

    for(int i=1;i<6;i++){
        week_list[i]->setAlignment(Qt::AlignCenter);
        week_list[i]->setText("周"+data_vec[i].m_week.remove(0,2));
    }
    for(int i=0;i<6;i++){
        date_list[i]->setAlignment(Qt::AlignCenter);
        date_list[i]->setText(data_vec[i].m_ymd.remove(0,5));
        wea_list[i]->setAlignment(Qt::AlignCenter);
        wea_list[i]->setText(data_vec[i].m_type);
        fx_list[i]->setAlignment(Qt::AlignCenter);
        fx_list[i]->setText(data_vec[i].m_fx);
        fl_list[i]->setAlignment(Qt::AlignCenter);
        fl_list[i]->setText(data_vec[i].m_fl);
    }
    //绘制曲线
    int high_sum=0,low_sum=0;
    for(int i=0;i<6;i++){
        high_sum+=data_vec[i].m_high;
        low_sum+=data_vec[i].m_low;
    }
    high_ave=high_sum/6;
    low_ave=low_sum/6;
    connect(this,&MainWindow::sigPaint,ui->curv_wid,&CurWidget::slotPaint);
    emit(sigPaint(quality_list,high_ave,low_ave,data_vec));
    //图标
    QPixmap pix;
    pix=QPixmap(":/fengli.png");
    pix=pix.scaled(80,80,Qt::KeepAspectRatio);
    ui->icon1_lab->setAlignment(Qt::AlignRight);
    ui->icon1_lab->setPixmap(pix);
    pix=QPixmap(":/pm25.png");
    pix=pix.scaled(80,80,Qt::KeepAspectRatio);
    ui->icon2_lab->setAlignment(Qt::AlignRight);
    ui->icon2_lab->setPixmap(pix);
    pix=QPixmap(":/shidu4.png");
    pix=pix.scaled(80,80,Qt::KeepAspectRatio);
    ui->icon3_lab->setAlignment(Qt::AlignRight);
    ui->icon3_lab->setPixmap(pix);
    pix=QPixmap(":/zhiliang.png");
    pix=pix.scaled(80,80,Qt::KeepAspectRatio);
    ui->icon4_lab->setAlignment(Qt::AlignRight);
    ui->icon4_lab->setPixmap(pix);
    setColor(data_vec,quality_list);
    setIcon(data_vec,icon_list);
}
//把ui里的控件放入容器中
void MainWindow::pushLabel()
{
    week_list<<ui->week1<<ui->week2<<ui->week3<<ui->week4<<ui->week5<<ui->week6;
    date_list<<ui->date1<<ui->date2<<ui->date3<<ui->date4<<ui->date5<<ui->date6;
    icon_list<<ui->icon1<<ui->icon2<<ui->icon3<<ui->icon4<<ui->icon5<<ui->icon6;
    wea_list<<ui->wea1<<ui->wea2<<ui->wea3<<ui->wea4<<ui->wea5<<ui->wea6;
    quality_list<<ui->quality1<<ui->quality2<<ui->quality3<<ui->quality4<<ui->quality5<<ui->quality6;
    fx_list<<ui->fx1<<ui->fx2<<ui->fx3<<ui->fx4<<ui->fx5<<ui->fx6;
    fl_list<<ui->fl1<<ui->fl2<<ui->fl3<<ui->fl4<<ui->fl5<<ui->fl6;
}

void MainWindow::setIcon(QVector<Dataset>vec,QList<QLabel*>list)
{
    if(vec.size()<=0||list.size()<=0){
        return;
    }
    QPixmap pix;
    for(int i=0;i<6;i++){
        list[i]->setAlignment(Qt::AlignCenter);
        if(vec[i].m_type=="暴雨"||vec[i].m_type=="暴雨到大暴雨"||vec[i].m_type=="大暴雨"||
                vec[i].m_type=="大暴雨到特大暴雨"||vec[i].m_type=="大到暴雨"){
            pix=QPixmap(":/baoyu.png");
            pix=pix.scaled(50,50,Qt::KeepAspectRatio);
            list[i]->setPixmap(pix);
            if(i==0){
                pix=pix.scaled(100,100,Qt::KeepAspectRatio);
                ui->weather_icon_lab->setPixmap(pix);
            }
        }
        else if(vec[i].m_type=="大雨"){
            pix=QPixmap(":/dayu.png");
            pix=pix.scaled(50,50,Qt::KeepAspectRatio);
            list[i]->setPixmap(pix);
            if(i==0){
                pix=pix.scaled(100,100,Qt::KeepAspectRatio);
                ui->weather_icon_lab->setPixmap(pix);
            }
        }
        else if(vec[i].m_type=="暴雪"||vec[i].m_type=="大到暴雪"||vec[i].m_type=="大雪"){
            pix=QPixmap(":/daxue.png");
            pix=pix.scaled(50,50,Qt::KeepAspectRatio);
            list[i]->setPixmap(pix);
            if(i==0){
                pix=pix.scaled(100,100,Qt::KeepAspectRatio);
                ui->weather_icon_lab->setPixmap(pix);
            }
        }
        else if(vec[i].m_type=="冻雨"){
            pix=QPixmap(":/dongyu.png");
            pix=pix.scaled(50,50,Qt::KeepAspectRatio);
            list[i]->setPixmap(pix);
            if(i==0){
                pix=pix.scaled(100,100,Qt::KeepAspectRatio);
                ui->weather_icon_lab->setPixmap(pix);
            }
        }
        else if(vec[i].m_type=="多云"){
            pix=QPixmap(":/duoyun.png");
            pix=pix.scaled(50,50,Qt::KeepAspectRatio);
            list[i]->setPixmap(pix);
            if(i==0){
                pix=pix.scaled(100,100,Qt::KeepAspectRatio);
                ui->weather_icon_lab->setPixmap(pix);
            }
        }
        else if(vec[i].m_type=="浮沉"){
            pix=QPixmap(":/fucehn.png");
            pix=pix.scaled(50,50,Qt::KeepAspectRatio);
            list[i]->setPixmap(pix);
            if(i==0){
                pix=pix.scaled(100,100,Qt::KeepAspectRatio);
                ui->weather_icon_lab->setPixmap(pix);
            }
        }
        else if(vec[i].m_type=="雷阵雨"||vec[i].m_type=="雷阵雨伴有冰雹"){
            pix=QPixmap(":/leidianyu.png");
            pix=pix.scaled(50,50,Qt::KeepAspectRatio);
            list[i]->setPixmap(pix);
            if(i==0){
                pix=pix.scaled(100,100,Qt::KeepAspectRatio);
                ui->weather_icon_lab->setPixmap(pix);
            }
        }
        else if(vec[i].m_type=="霾"){
            pix=QPixmap(":/wumai.png");
            pix=pix.scaled(50,50,Qt::KeepAspectRatio);
            list[i]->setPixmap(pix);
            if(i==0){
                pix=pix.scaled(100,100,Qt::KeepAspectRatio);
                ui->weather_icon_lab->setPixmap(pix);
            }
        }
        else if(vec[i].m_type=="强沙尘暴"){
            pix=QPixmap(":/shachenbao.png");
            pix=pix.scaled(50,50,Qt::KeepAspectRatio);
            list[i]->setPixmap(pix);
            if(i==0){
                pix=pix.scaled(100,100,Qt::KeepAspectRatio);
                ui->weather_icon_lab->setPixmap(pix);
            }
        }
        else if(vec[i].m_type=="强沙尘暴"){
            pix=QPixmap(":/shachenbao.png");
            pix=pix.scaled(50,50,Qt::KeepAspectRatio);
            list[i]->setPixmap(pix);
            if(i==0){
                pix=pix.scaled(100,100,Qt::KeepAspectRatio);
                ui->weather_icon_lab->setPixmap(pix);
            }
        }
        else if(vec[i].m_type=="晴"){
            pix=QPixmap(":/qing.png");
            pix=pix.scaled(50,50,Qt::KeepAspectRatio);
            list[i]->setPixmap(pix);
            if(i==0){
                pix=pix.scaled(100,100,Qt::KeepAspectRatio);
                ui->weather_icon_lab->setPixmap(pix);
            }
        }
        else if(vec[i].m_type=="小雨"){
            pix=QPixmap(":/xiaoyu.png");
            pix=pix.scaled(50,50,Qt::KeepAspectRatio);
            list[i]->setPixmap(pix);
            if(i==0){
                pix=pix.scaled(100,100,Qt::KeepAspectRatio);
                ui->weather_icon_lab->setPixmap(pix);
            }
        }
        else if(vec[i].m_type=="阴"){
            pix=QPixmap(":/yin.png");
            pix=pix.scaled(50,50,Qt::KeepAspectRatio);
            list[i]->setPixmap(pix);
            if(i==0){
                pix=pix.scaled(100,100,Qt::KeepAspectRatio);
                ui->weather_icon_lab->setPixmap(pix);
            }
        }
    }
}

void MainWindow::setColor(QVector<Dataset>vec,QList<QLabel*>list)
{
    if(vec.size()<=0||list.size()<=0){
        return;
    }
    for(int i=0;i<6;i++){
        list[i]->setAlignment(Qt::AlignCenter);
        int index=vec[i].m_aqi;
        if(index>=0&&index<=50){
            list[i]->setText("优");
            list[i]->setStyleSheet("QLabel{background-color:rgb(224, 255, 65);"
                                   "border-radius:6px;"
                                   "text-align:center;vertical-align:middle;}");

        }
        else if(index>50&&index<=100){
            list[i]->setText("良");
            list[i]->setStyleSheet("QLabel{background-color: rgb(255, 243, 169);"
                                   "border-radius:6px;"
                                   "text-align:center;vertical-align:middle;}");

        }
        else if(index>100&&index<=150){
            list[i]->setText("中度污染");
            list[i]->setStyleSheet("QLabel{background-color: rgb(254, 84, 127);"
                                   "border-radius:6px;"
                                   "text-align:center;vertical-align:middle;}");

        }
        else if(index>150&&index<=200){
            list[i]->setText("重度污染");
            list[i]->setStyleSheet("QLabel{background-color: rgb(170, 0, 0);"
                                   "border-radius:6px;"
                                   "text-align:center;vertical-align:middle;}");

        }
    }
}



void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    QPoint mouse_point=event->globalPos();
    this->move(mouse_point-relative_point);
    return QMainWindow::mouseMoveEvent(event);
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    relative_point=event->globalPos()-this->pos();
    return QMainWindow::mousePressEvent(event);
}
//双击收缩下拉
void MainWindow::mouseDoubleClickEvent(QMouseEvent *event)
{
    qint64 current_time=QDateTime::currentMSecsSinceEpoch();
    if(current_time-last_time<=900){
        last_time=current_time;
        return;
    }
    last_time=current_time;
    if(!isUp){
        QPropertyAnimation* up1=new QPropertyAnimation(this,"size");
        up1->setStartValue(this->size());
        up1->setEndValue(QSize(this->width(),
                       this->height()-ui->last_wid->height()));
        up1->setDirection(QAbstractAnimation::Direction::Forward);
        up1->setDuration(500);

        QPropertyAnimation* up2=new QPropertyAnimation(this,"size");
        up2->setStartValue(QSize(this->width(),this->height()-ui->last_wid->height()));
        up2->setEndValue(QSize(this->width(),
                       this->height()-ui->last_wid->height()-ui->weainfo_wid->height()));
        up2->setDirection(QAbstractAnimation::Direction::Forward);
        up2->setDuration(300);
        up1->start();
        QTimer* time=new QTimer;
        time->setSingleShot(true);
        time->start(600);
        connect(time,&QTimer::timeout,this,[=](){
            up2->start();
            isUp=true;
            delete time;
        });
        time=nullptr;

    }
    else{
        QPropertyAnimation* down1=new QPropertyAnimation(this,"size");
        down1->setStartValue(QSize(this->width(),this->height()));
        down1->setEndValue(QSize(this->width(),
                       this->height()+m_second_height));
        down1->setDirection(QAbstractAnimation::Direction::Forward);
        down1->setDuration(500);

        QPropertyAnimation* down2=new QPropertyAnimation(this,"size");
        down2->setStartValue(QSize(this->width(),this->height()+m_second_height));
        down2->setEndValue(QSize(this->width(),
                       this->height()+m_second_height+m_last_height));
        down2->setDirection(QAbstractAnimation::Direction::Forward);
        down2->setDuration(300);
        down1->start();
        QTimer* time=new QTimer;
        time->setSingleShot(true);
        time->start(600);
        connect(time,&QTimer::timeout,this,[=](){
            down2->start();
            isUp=false;
            delete time;
        });
        time=nullptr;
    }
    return QMainWindow::mouseDoubleClickEvent(event);
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    return QMainWindow::paintEvent(event);
}

void MainWindow::slotClose()
{
    qApp->exit(0);
}
//处理应答
void MainWindow::slotHandleReply(QNetworkReply *reply)
{
    if(reply==nullptr){
        return;
    }
    int state_code=reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if(state_code!=200||reply->error()!=QNetworkReply::NoError){
        qDebug()<<"pppp";
        qDebug()<<state_code;
        QMessageBox::warning(this,"数据请求","数据请求失败",QMessageBox::Ok);
        return;
    }
    QByteArray raw_date=reply->readAll();
    parseJson(raw_date);
    reply->deleteLater();

}

void MainWindow::slotUpdate()
{
    QString search=ui->lineEdit->text();
    QChar last=search.back();
    QChar b=L'市';
    if(map[search]==""&&last!=b){
        search=search+"市";
        if(map[search]!=""){
            getWeatherInfo(map[search]);
        }
        else{
            QMessageBox::warning(this,"查询","无效地址",QMessageBox::Ok);
        }
    }
    else if(map[search]==""&&last==b){
        search=search.remove(-1,1);
        if(map[search]!=""){
            getWeatherInfo(map[search]);
        }
        else{
            QMessageBox::warning(this,"查询","无效地址",QMessageBox::Ok);
        }
    }
    else{
        getWeatherInfo(map[search]);
    }
    //m_city_name=search;

}

/*void MainWindow::slotTimeUpdate()
{
    qDebug()<<"p";
    qDebug()<<m_city_name;
    getWeatherInfo(m_city_name);
}*/
