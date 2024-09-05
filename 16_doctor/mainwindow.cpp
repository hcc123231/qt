#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QComboBox>
#include<QFile>
#include<QTextStream>
#include<QDebug>
#include<QTime>
#include<QDate>
#include<QKeyEvent>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //设置各类控件的样式
    setQss(ui);

    ui->hos_na_cbox->installEventFilter(this);
    //设置QComboBox的插入策略为不自动插入，因为自动插入会影响后面的findText的判断
    ui->hos_na_cbox->setInsertPolicy(QComboBox::NoInsert);
    //处理医院名称
    //读取文件内容获取行数并逐行将内容设置在hos_na_cbox上
    QFile hos_na_file("C:\\Users\\hcc\\Documents\\16_doctor\\hos_name.conf");
    if(!hos_na_file.open(QIODevice::ReadOnly)){
        qDebug("failed to open hos_na_file");
        return;
    }
    QTextStream hos_na_stream(&hos_na_file);
    int file_count=0;
    while(!hos_na_stream.atEnd()){
        QString new_hos_na=hos_na_stream.readLine();
        ui->hos_na_cbox->addItem(new_hos_na);
        file_count++;
    }
    hos_na_file.close();
    //获取当前时间并且设置进三个QTimeEdit中
    ui->collection_time->setTime(QTime::currentTime());
    ui->admission_time->setTime(QTime::currentTime());
    ui->record_time->setTime(QTime::currentTime());
    //获取当前日期并且将日期放入三个QDateEDit中
    ui->collection_date->setDate(QDate::currentDate());
    ui->admission_date->setDate(QDate::currentDate());
    ui->record_date->setDate(QDate::currentDate());
    //为性别ComboBox添加几个默认选项
    ui->gender_cbox->addItem("/");
    ui->gender_cbox->addItem("男");
    ui->gender_cbox->addItem("女");
    ui->gender_cbox->addItem("不详");

    //为婚否ComboBox添加几个默认选项
    ui->marry_cbox->addItem("/");
    ui->marry_cbox->addItem("已婚");
    ui->marry_cbox->addItem("未婚");
    ui->marry_cbox->addItem("不详");


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setQss(Ui::MainWindow *ui)
{
    if(ui==nullptr){
        return;
    }
    ui->title_lab->setText("病历记录");
    ui->title_lab->setAlignment(Qt::AlignHCenter);
    ui->title_lab->setStyleSheet("QLabel{font:13pt '楷体'}");
    ui->hos_na_cbox->setEditable(true);
    ui->admission_date->setDisplayFormat("yyyy-MM-dd");
    ui->admission_time->setDisplayFormat("hh:mm:ss");
    ui->collection_date->setDisplayFormat("yyyy-MM-dd");
    ui->collection_time->setDisplayFormat("hh:mm:ss");
    ui->record_date->setDisplayFormat("yyyy-MM-dd");
    ui->record_time->setDisplayFormat("hh:mm:ss");
    ui->native_lab->setWordWrap(true);
    ui->method1_cbox->setMinimumHeight(45);
    ui->method2_cbox->setMinimumHeight(45);
    ui->method3_cbox->setMinimumHeight(45);
}


bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if(watched==nullptr||event==nullptr){
        return 1;
    }
    if(watched==ui->hos_na_cbox&&event->type()==QEvent::KeyPress){

        QKeyEvent* keyEvent=dynamic_cast<QKeyEvent*>(event);
        if(keyEvent->key()==Qt::Key_Return||keyEvent->key()==Qt::Key_Enter){
            qDebug("q");
            QString hos_na_text=ui->hos_na_cbox->currentText();
            if(ui->hos_na_cbox->findText(hos_na_text,Qt::MatchExactly)!=-1){
                qDebug("w");
                return 1;
            }
            else{
                qDebug("e");
                QFile file("C:\\Users\\hcc\\Documents\\16_doctor\\hos_name.conf");
                if(!file.open(QIODevice::WriteOnly|QIODevice::Append|QIODevice::Text)){
                    file.close();
                    qDebug("r");
                    return 1;
                }
                else{
                    qDebug("t");
                    ui->hos_na_cbox->addItem(hos_na_text);
                    QTextStream stream(&file);
                    stream<<hos_na_text<<'\n';
                    file.close();
                    return 1;
                }
            }
        }

    }
    return QWidget::eventFilter(watched,event);
}
