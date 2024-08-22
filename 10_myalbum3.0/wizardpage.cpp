#include "wizardpage.h"
#include "ui_wizardpage.h"
#include<QDir>
#include<QFileDialog>
#include<QDebug>
WizardPage::WizardPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::WizardPage)
{
    ui->setupUi(this);
    registerField("proName*",ui->file_name_edit);
    registerField("proPath*",ui->file_path_edit);
    QString path=QDir::currentPath();
    ui->file_path_edit->setText(path);
    connect(ui->browse_btn,&QPushButton::clicked,this,&WizardPage::onClicked);
    connect(ui->file_name_edit,&QLineEdit::textChanged,this,&WizardPage::checkInfo);
    connect(ui->file_path_edit,&QLineEdit::textChanged,this,&WizardPage::checkInfo);
    checkInfo();

}

WizardPage::~WizardPage()
{
    delete ui;
}

void WizardPage::checkInfo()
{
    //检查用户输入文件路径是否否存在，检查文件路径连接起来是否存在，要的是文件路径存在，文件路径连起来不存在
    QString file_path=ui->file_path_edit->text();
    QDir file_dir(file_path);
    QString abs_file_path=file_dir.absoluteFilePath(ui->file_name_edit->text());
    QFileInfo fileinfo=file_path;
    QFileInfo fileinfo2=abs_file_path;
    ui->tips_lab->setText("");
    if(fileinfo.exists()&&!fileinfo2.exists())
    {
        isExist=true;
    }
    else {
        isExist=false;
        if(fileinfo.exists()&&fileinfo2.exists()&&!(ui->file_name_edit->text()=="")){
            ui->tips_lab->setText("文件路径已存在，请重新选择");
        }
        else if(!fileinfo.exists()){
            ui->tips_lab->setText("文件路径不存在，请重新选择");
        }

    }
}

Ui::WizardPage *WizardPage::getui()
{
    return ui;
}
bool WizardPage::isComplete() const
{
    if(ui->file_name_edit->text()==""||ui->file_path_edit->text()=="")
    {
        return false;
    }
    if(isExist){
        return true;
    }
    else{
        return false;
    }
}

void WizardPage::onClicked()
{
    QString path=QDir::currentPath();
    QString new_path=QFileDialog::getExistingDirectory(this,"选择文件",path);
    if(new_path==""){
        ui->file_path_edit->setText(path);
    }
    else ui->file_path_edit->setText(new_path);
}

