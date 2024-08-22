#ifndef WIDGET_H
#define WIDGET_H
#include<QWidget>
#include<QToolButton>
class Widget : public QWidget
{
    Q_OBJECT
public:
    explicit Widget(QWidget *parent = nullptr);
    void setBUtton(QToolButton* pre_btn,QToolButton* next_btn);
signals:
protected:
    virtual void enterEvent(QEvent *event);
    virtual void leaveEvent(QEvent *event);
public slots:
private:
    QToolButton* _pre_btn=nullptr;
    QToolButton* _next_btn=nullptr;
};

#endif // WIDGET_H
