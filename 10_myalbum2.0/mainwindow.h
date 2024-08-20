#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include"wizardpage.h"
#include <QMainWindow>
#include<QTreeWidgetItem>
#include"filethread.h"
#include"treewidgetitem.h"
#include"picshowwind.h"
#include<QMediaPlayer>
#include<QMediaPlaylist>
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
    PicShowWind* picshowwind;
    QMediaPlayer* mus_player;
public slots:
    void slotProCreate();
    void onAccepted();
    void slotProOpen();
    void slotItemRightPressed(QTreeWidgetItem *item, int column);
    void slotDoubleClicked(QTreeWidgetItem *item, int column);
    void slotPreDispaly(QTreeWidget* tree_widget,MainWindow* mainwindow);
    void slotNextDispaly(QTreeWidget* tree_widget,MainWindow* mainwindow);
    void slotCloseFiles(TreeWidgetItem* item);
    void slotImportFiles();
    void slotImportMusic();
};

#endif // MAINWINDOW_H
