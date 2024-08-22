#ifndef WIZARDPAGE_H
#define WIZARDPAGE_H

#include <QWizardPage>

namespace Ui {
class WizardPage;
}

class WizardPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit WizardPage(QWidget *parent = nullptr);
    ~WizardPage();
    void checkInfo();
    Ui::WizardPage *getui();
protected:
    virtual bool isComplete() const;
public slots:
    void onClicked();
private:
    Ui::WizardPage *ui;
    bool isExist=false;

};

#endif // WIZARDPAGE_H
