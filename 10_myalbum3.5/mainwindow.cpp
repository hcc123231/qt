#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"ui_wizardpage.h"
#include<QMenu>
#include<QAction>
#include<QWizard>
#include<QWizardPage>
#include<QVBoxLayout>
#include<QHBoxLayout>
#include<QLabel>
#include<QLineEdit>
#include<QPushButton>
#include<QSpacerItem>
#include<QDebug>
#include<QObject>
#include<QDir>
#include<QFileDialog>
#include<QGuiApplication>
#include<QCursor>
#include"dispalypicthread.h"
#include"treewidgetitem.h"
#include"importthread.h"
#include<QtConcurrent/QtConcurrent>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->pre_btn->hide();
    ui->next_btn->hide();
    ui->pre_btn->setIcon(QIcon(":/7a7cadbdc5804387a0af64441d540856.png"));
    ui->pre_btn->setIconSize(QSize(80,80));
    ui->next_btn->setIcon(QIcon(":/71f0e1cfea8f4fafbf0051ca362b9a5f.png"));
    ui->next_btn->setIconSize(QSize(80,80));
    ui->pre_btn->setStyleSheet("QToolButton{background-color:rgb(77,77,77);"
                               "border:0px;}");
    ui->next_btn->setStyleSheet("QToolButton{background-color:rgb(77,77,77);"
                               "border:0px;}");

    ui->pre_btn->setShortcut(QKeySequence(Qt::Key_Left));
    ui->next_btn->setShortcut(QKeySequence(Qt::Key_Right));
    ui->treeWidget->header()->hide();
    ui->lab_lst_title->setText("文件列表");
    //创建两个菜单
    QMenu* pro_menu=new QMenu("项目",this);
    QMenu* set_menu=new QMenu("设置",this);
    pro_menu->setStyleSheet("QMenu{background-color: rgb(181, 224, 217);}");
    set_menu->setStyleSheet("QMenu{background-color: rgb(181, 224, 217);}");
    ui->menuBar->addMenu(pro_menu);
    ui->menuBar->addMenu(set_menu);
    //创建几个动作
    QAction* pro_create_action=new QAction("创建项目",this);
    QAction* pro_open_action=new QAction("打开项目",this);
    QAction* import_music_action=new QAction("导入音乐",this);
    set_menu->addAction(import_music_action);
    pro_menu->addAction(pro_create_action);
    pro_menu->addAction(pro_open_action);
    //设置图标
    pro_create_action->setIcon(QIcon(":/bc892dda8bfa486d927d5cca73d5ed18.png"));
    pro_open_action->setIcon(QIcon(":/c9601c011a794930bf44d6d9c2ad5a23.png"));
    import_music_action->setIcon(QIcon(":/8824274a591348f38dc73d059c66a03f.png"));
    //为动作设置快捷键
    pro_create_action->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_K));
    pro_open_action->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_O));
    import_music_action->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_M));
    //连接"创建项目"动作信号与槽
    connect(pro_create_action, &QAction::triggered, this, &MainWindow::slotProCreate);
    connect(pro_open_action, &QAction::triggered, this, &MainWindow::slotProOpen);
    connect(import_music_action,&QAction::triggered,this,&MainWindow::slotImportMusic);
    manageItemClicked(ui->treeWidget);
    //双击Item显示图片
    connect(ui->treeWidget,&QTreeWidget::itemDoubleClicked,this,
            &MainWindow::slotDoubleClicked);
    //点击前一张按钮
    connect(ui->pre_btn,&QToolButton::clicked,this,[=](){
        this->slotPreDispaly(ui->treeWidget,this);
    });
    //点击下一张按钮
    connect(ui->next_btn,&QToolButton::clicked,this,[=](){
        this->slotNextDispaly(ui->treeWidget,this);
    });
    connect(ui->treeWidget,&QTreeWidget::itemExpanded,this,[=](){
        TreeWidgetItem* icon_item=dynamic_cast<TreeWidgetItem*>
                (ui->treeWidget->currentItem());
        icon_item->setData(0,Qt::DecorationRole,QIcon(":/4f71917b12b241a8a18c69d60d52c7b1.png"));
    });

    ui->lab_lst_title->setStyleSheet("QLabel{border-bottom:1px solid white}");
    ui->lab_lst_title->update();

}


MainWindow::~MainWindow()
{
    delete ui;

}

void MainWindow::manageItemClicked(QTreeWidget *tree_widget)
{
    tree_widget=ui->treeWidget;
    connect(tree_widget,&QTreeWidget::itemPressed,this,
            &MainWindow::slotItemRightPressed);
}

Ui::MainWindow *MainWindow::getui()
{
    return ui;
}


void MainWindow::slotProCreate()
{
    QWizard* wizard=new QWizard(this);
    wizard->setWindowTitle("创建项目");
    page1=new WizardPage(wizard);
    QWizardPage* page2=new QWizardPage(wizard);
    page1->setTitle("选择文件");
    wizard->addPage(page1);
    wizard->addPage(page2);
    //创建wizard的第二页的界面
    QLabel* finish_lab=new QLabel(page2);
    finish_lab->setText("文件已创建成功，点击finish退出，点击cancel结束");
    QVBoxLayout* vlayout2=new QVBoxLayout;
    vlayout2->addWidget(finish_lab);
    page2->setLayout(vlayout2);
    wizard->show();
    //连接wizard的finish信号与槽
    connect(wizard,&QWizard::accepted,this,&MainWindow::onAccepted);

}

void MainWindow::onAccepted()
{
    //在本地磁盘上创建文件夹
    QString path=page1->getui()->file_path_edit->text();
    QString name=page1->getui()->file_name_edit->text();
    QDir path_dir=path;
    QString final_path=path_dir.absoluteFilePath(name);
    path_dir.mkpath(final_path);
    QFileInfo fileinfo(final_path);
    TreeWidgetItem* top_item=new TreeWidgetItem(ui->treeWidget);
    top_item->setPre(nullptr);
    top_item->setNext(nullptr);
    top_item->setData(0,Qt::DisplayRole,fileinfo.fileName());
    top_item->setData(0,Qt::DecorationRole,QIcon(":/6a9c6d12bd6c4a53b27e4cd35dfc8b3b.png"));
    top_item->setToolTip(0,final_path);
    ui->treeWidget->addTopLevelItem(top_item);
    thread=new FileThread(this,final_path,top_item,ui->treeWidget);
    thread->start();
    connect(thread,&QThread::finished,thread,[=](){
       thread->exit();
       thread->wait();
       thread->deleteLater();
    });

}

void MainWindow::slotProOpen()
{
    //点击打开项目的槽函数,将文件展开在treeWidget中
    QString path=QDir::currentPath();
    QString select_path=QFileDialog::getExistingDirectory(this,"选择文件夹",path);
    if(select_path==""){
        return;
    }

    QFileInfo fileinfo(select_path);
    TreeWidgetItem* top_item=new TreeWidgetItem(ui->treeWidget);
    top_item->setPre(nullptr);
    top_item->setNext(nullptr);
    top_item->setData(0,Qt::DisplayRole,fileinfo.fileName());
    top_item->setData(0,Qt::DecorationRole,QIcon(":/6a9c6d12bd6c4a53b27e4cd35dfc8b3b.png"));
    top_item->setToolTip(0,select_path);
    ui->treeWidget->addTopLevelItem(top_item);
    thread=new FileThread(this,select_path,top_item,ui->treeWidget);
    thread->start();
    connect(thread,&QThread::finished,thread,[=](){
       thread->exit();
       thread->wait();
       thread->deleteLater();
    });
}
//不知可否将QTreeWidgetItem*转换成TreeWidgetItem*类型
void MainWindow::slotItemRightPressed(QTreeWidgetItem *item,int column)
{
    QVariant tooltip=item->toolTip(column);
    QString s=tooltip.toString();
    if(QGuiApplication::mouseButtons()==Qt::RightButton)
    {
        QFileInfo fileinfo(s);
        if(fileinfo.isDir())
        {
            btn_menu=new QMenu(this);
            btn_menu->setStyleSheet("QMenu{background-color:rgb(181, 224, 217);}");
            QAction* import_file_act=new QAction(tr("导入文件夹"),btn_menu);
            QAction* close_file_act=new QAction(tr("关闭文件"),btn_menu);
            QAction* slide_show_act=new QAction(tr("轮播图播放"),btn_menu);

            import_file_act->setIcon(QIcon(":/aa05e798e5234e54a14eebe7e25becee.png"));
            close_file_act->setIcon(QIcon(":/b8683a4f8d1643fd8a9fed80c105472c.png"));
            slide_show_act->setIcon(QIcon(":/694dff865c944690849be1b904505db7.png"));

            btn_menu->addAction(import_file_act);
            btn_menu->addAction(close_file_act);
            btn_menu->addAction(slide_show_act);

            //导入文件
            connect(import_file_act,&QAction::triggered,this,&MainWindow::slotImportFiles);
            //关闭文件
            connect(close_file_act,&QAction::triggered,this,[=](){
                TreeWidgetItem* current_item=dynamic_cast<TreeWidgetItem*>
                        (ui->treeWidget->currentItem());
                this->slotCloseFiles(dynamic_cast<TreeWidgetItem*>
                                     (current_item));

                ui->treeWidget->removeItemWidget(current_item,0);
                if(current_item){
                    delete current_item;
                }
            });

            //轮播图
            connect(slide_show_act,&QAction::triggered,this,[=](){
                TreeWidgetItem* item=dynamic_cast<TreeWidgetItem*>(ui->treeWidget
                                                                    ->currentItem());
                picshowwind=new PicShowWind(mus_player,item,this);
            });
            btn_menu->exec(QCursor::pos());

        }
    }

}

void MainWindow::slotDoubleClicked(QTreeWidgetItem *item, int column)
{
    TreeWidgetItem* real_item=dynamic_cast<TreeWidgetItem*>(item);
    QFileInfo info(real_item->getPath());
    if(info.isDir()){
        return;
    }

    DispalyPicThread* disp_thread=new DispalyPicThread(this,ui->treeWidget,this);
    disp_thread->dispaly(ui->treeWidget,this,real_item,column);
    ui->widget->setBUtton(ui->pre_btn,ui->next_btn);
    connect(disp_thread,&QThread::finished,this,[=](){
        disp_thread->exit();
        disp_thread->wait();
        disp_thread->deleteLater();
    });
}

void MainWindow::slotPreDispaly(QTreeWidget* tree_widget,MainWindow* mainwindow)
{
    //先获取当前画面位置
    QTreeWidgetItem* item= mainwindow->getui()->treeWidget->currentItem();
    TreeWidgetItem* real_item=dynamic_cast<TreeWidgetItem*>(item);
    if(real_item->getPre()==nullptr){
        return;
    }
    QVariant vari=real_item->getPre()->toolTip(0);
    if(vari.isNull())
    {
        qDebug()<<"vari is null";
        return;
    }
    QString file_path=vari.toString();
    QFile pic_file(file_path);
    if(!pic_file.open(QFile::ReadOnly)){
        qDebug()<<"failed to open file";
        return;
    }
    QByteArray file_byte=pic_file.readAll();
    pic_file.close();
    QPixmap pix;
    pix.loadFromData(file_byte);
    mainwindow->getui()->dispaly_lab->setPixmap(pix);
    mainwindow->getui()->dispaly_lab->setFixedSize(1800,1100);
    mainwindow->getui()->dispaly_lab->setScaledContents(true);
    //设置新的当前位置
    tree_widget->setCurrentItem(real_item->getPre());
}

void MainWindow::slotNextDispaly(QTreeWidget *tree_widget, MainWindow *mainwindow)
{
    //先获取当前画面位置
    QTreeWidgetItem* item= mainwindow->getui()->treeWidget->currentItem();
    TreeWidgetItem* real_item=dynamic_cast<TreeWidgetItem*>(item);
    if(real_item->getNext()==nullptr){
        return;
    }
    QVariant vari=real_item->getNext()->toolTip(0);
    if(vari.isNull())
    {
        qDebug()<<"vari is null";
        return;
    }
    QString file_path=vari.toString();
    QFile pic_file(file_path);
    if(!pic_file.open(QFile::ReadOnly)){
        qDebug()<<"failed to open file";
        return;
    }
    QByteArray file_byte=pic_file.readAll();
    pic_file.close();
    QPixmap pix;
    pix.loadFromData(file_byte);
    mainwindow->getui()->dispaly_lab->setPixmap(pix);
    mainwindow->getui()->dispaly_lab->setFixedSize(1800,1100);
    mainwindow->getui()->dispaly_lab->setScaledContents(true);
    //设置新的当前位置
    tree_widget->setCurrentItem(real_item->getNext());
}

void MainWindow::slotCloseFiles(TreeWidgetItem *item)
{
    int count=item->childCount();
    qDebug()<<count;
    for(int i=count-1;i>=0;i--){

        TreeWidgetItem* son_item=dynamic_cast<TreeWidgetItem*>(item->child(i));
        if(son_item->childCount()){
            slotCloseFiles(son_item);
        }
        item->removeChild(son_item);
        if(son_item){
            delete son_item;
        }
    }
}

void MainWindow::slotImportFiles()
{
    QString selected=QFileDialog::getExistingDirectory
            (this,"导入文件夹",QDir::currentPath());
    if(selected==""){
        return;
    }
    TreeWidgetItem* root_item=dynamic_cast<TreeWidgetItem*>(ui->treeWidget->
                                      currentItem());
    ImportThread* thread=new ImportThread(root_item->getPath(),root_item,root_item->getPath(),selected,this);
    thread->start();

    connect(thread,&QThread::finished,this,[=](){
        thread->exit();
        thread->wait();
        thread->deleteLater();
    });

}

void MainWindow::slotImportMusic()
{
    QString selected=QFileDialog::getOpenFileName(this,"导入音乐",QDir::currentPath(),
                                 "*.mp3 *.wav *.mp4");
    if(selected==""){
        return;
    }
    mus_player=new QMediaPlayer(this);
    mus_player->setMedia(QUrl::fromLocalFile(selected));


}
