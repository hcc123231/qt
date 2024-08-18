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
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->treeWidget->header()->hide();
    ui->lab_lst_title->setText("文件列表");
    //创建两个菜单
    QMenu* pro_menu=new QMenu("项目",this);
    QMenu* set_menu=new QMenu("设置",this);
    ui->menuBar->addMenu(pro_menu);
    ui->menuBar->addMenu(set_menu);
    //创建几个动作
    QAction* pro_create_action=new QAction("创建项目",this);
    QAction* pro_open_action=new QAction("打开项目",this);
    pro_menu->addAction(pro_create_action);
    pro_menu->addAction(pro_open_action);
    //为动作设置快捷键
    pro_create_action->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_M));
    pro_open_action->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_O));
    //连接"创建项目"动作信号与槽
    connect(pro_create_action, &QAction::triggered, this, &MainWindow::slotProCreate);
    connect(pro_open_action, &QAction::triggered, this, &MainWindow::slotProOpen);
    manageItemClicked(ui->treeWidget);

    connect(ui->treeWidget,&QTreeWidget::itemDoubleClicked,this,
            &MainWindow::slotDoubleClicked);

}


MainWindow::~MainWindow()
{
    delete ui;

}

void MainWindow::manageItemClicked(QTreeWidget *tree_widget)
{
    tree_widget=ui->treeWidget;
    connect(tree_widget,&QTreeWidget::itemPressed,this,&MainWindow::slotItemPressed);
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
    QTreeWidgetItem* top_item=new QTreeWidgetItem(ui->treeWidget);
    //top_item->setPre(nullptr);
    top_item->setData(0,Qt::DisplayRole,fileinfo.fileName());
    top_item->setToolTip(0,final_path);
    ui->treeWidget->addTopLevelItem(top_item);
    thread=new FileThread(this,final_path,top_item);
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
    QFileInfo fileinfo(select_path);
    QTreeWidgetItem* top_item=new QTreeWidgetItem(ui->treeWidget);
    top_item->setData(0,Qt::DisplayRole,fileinfo.fileName());
    top_item->setToolTip(0,select_path);
    ui->treeWidget->addTopLevelItem(top_item);
    thread=new FileThread(this,select_path,top_item);
    thread->start();
    connect(thread,&QThread::finished,thread,[=](){
       thread->exit();
       thread->wait();
       thread->deleteLater();
    });
}

void MainWindow::slotItemPressed(QTreeWidgetItem *item,int column)
{
    QVariant tooltip=item->toolTip(column);
    QString s=tooltip.toString();
    if(QGuiApplication::mouseButtons()==Qt::RightButton)
    {
        QFileInfo fileinfo(s);
        if(fileinfo.isDir())
        {
            btn_menu=new QMenu(this);
            QAction* import_file_act=new QAction(tr("导入文件"),btn_menu);
            btn_menu->addAction(import_file_act);
            btn_menu->exec(QCursor::pos());

        }
    }

}

void MainWindow::slotDoubleClicked(QTreeWidgetItem *item, int column)
{
    DispalyPicThread* disp_thread=new DispalyPicThread(this,ui->treeWidget,this);
    disp_thread->dispaly(ui->treeWidget,this,item,column);
    connect(disp_thread,&QThread::finished,this,[=](){
        disp_thread->exit();
        disp_thread->wait();
        disp_thread->deleteLater();
    });
}
