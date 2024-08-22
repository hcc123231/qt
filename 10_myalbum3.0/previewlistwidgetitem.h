#ifndef PREVIEWLISTWIDGETITEM_H
#define PREVIEWLISTWIDGETITEM_H
#include<QListWidgetItem>

class PreViewListWidgetItem : public QListWidgetItem
{
public:
    explicit PreViewListWidgetItem(const QIcon &icon, const QString &text,
                             QListWidget *view = nullptr, int type = Type);
};

#endif // PREVIEWLISTWIDGETITEM_H
