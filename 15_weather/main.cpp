#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setStyleSheet("QWidget {"
                              "  border-radius: 20px;"  // 设置圆角半径为20像素
                              "  border: 1px solid black;"  // 设置边框颜色和宽度
                              "}");
    w.show();

    return a.exec();
}
