#ifndef WIZARDPAGE1_H
#define WIZARDPAGE1_H

#include <QWizardPage>

namespace Ui {
class WizardPage1;
}

class WizardPage1 : public QWizardPage
{
    Q_OBJECT

public:
    explicit WizardPage1(QWidget *parent = nullptr);
    ~WizardPage1();

private:
    Ui::WizardPage1 *ui;
};

#endif // WIZARDPAGE1_H
