#include "wizardpage1.h"
#include "ui_wizardpage1.h"

WizardPage1::WizardPage1(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::WizardPage1)
{
    ui->setupUi(this);
}

WizardPage1::~WizardPage1()
{
    delete ui;
}
