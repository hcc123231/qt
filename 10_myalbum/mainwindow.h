#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include"wizardpage.h"
#include <QMainWindow>
#include<QTreeWidgetItem>
#include"filethread.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void manageItemClicked(QTreeWidget* tree_widget);
    Ui::MainWindow *getui();
private:
    Ui::MainWindow *ui;
    WizardPage* page1;
    FileThread* thread;
    QMenu* btn_menu;

public slots:
    void slotProCreate();
    void onAccepted();
    void slotProOpen();
    void slotItemPressed(QTreeWidgetItem *item, int column);
    void slotDoubleClicked(QTreeWidgetItem *item, int column);
};

#endif // MAINWINDOW_H
