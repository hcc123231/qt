#include "wizard.h"
#include "ui_wizard.h"
#include<QDebug>
Wizard::Wizard(QWidget *parent) :
    QWizard(parent),
    ui(new Ui::Wizard)
{
    ui->setupUi(this);
}

Wizard::~Wizard()
{
    delete ui;
}


/*void Wizard::reject()
{
    qDebug()<<"ioio";
}*/
