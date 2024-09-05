#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QEvent>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setQss(Ui::MainWindow* ui);
public slots:

private:
    Ui::MainWindow *ui;
protected:
    bool eventFilter(QObject* watched,QEvent* event)override;

};

#endif // MAINWINDOW_H
