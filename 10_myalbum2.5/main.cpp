#include "mainwindow.h"
#include <QApplication>
#include<QFile>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    QFile file(":/style.qss");
    if(file.open(QFile::ReadOnly)){
        QString style=file.readAll();
        a.setStyleSheet(style);
        file.close();
    }else{
        qDebug()<<"failed to open qss";
    }

    w.showMaximized();

    return a.exec();
}
