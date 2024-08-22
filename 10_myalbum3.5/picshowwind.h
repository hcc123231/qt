#ifndef PICSHOWWIND_H
#define PICSHOWWIND_H
#include<QPixmap>
#include <QDialog>
#include"treewidgetitem.h"
#include<QMediaPlayer>
#include<QMediaPlaylist>
namespace Ui {
class PicShowWind;
}

class PicShowWind : public QDialog
{
    Q_OBJECT

public:
    explicit PicShowWind(QMediaPlayer* mus_player,TreeWidgetItem* root_item,QWidget *parent = nullptr);
    ~PicShowWind();
protected:
    void closeEvent(QCloseEvent *)override;
private:
    Ui::PicShowWind *ui;
    TreeWidgetItem* _root_item=nullptr;
    QMediaPlayer* _mus_player=nullptr;
    int _pic_idx=0;
    bool isStop=false;
private slots:
signals:
    void sigAdjustSPeed(double speed);

};

#endif // PICSHOWWIND_H
