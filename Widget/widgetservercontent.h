#ifndef WIDGETSERVERCONTENT_H
#define WIDGETSERVERCONTENT_H

#include <QWidget>

#include "Public/defines.h"

namespace Ui {
class WidgetServerContent;
}

class NetworkObject;
class WidgetServerContent : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetServerContent(NetworkObject* netObj, QWidget *parent = nullptr);
    ~WidgetServerContent();

private:
    void init();

private:
    QString getServerStatusText(int status);

private slots:
    void slot_update_network_object(const QString &token);
    void slot_recreate_network_object(const QString &token, const QString &address, uint16_t port);

private:
    Ui::WidgetServerContent *ui;
    NetworkObject *mNetworkObject = nullptr;
};

#endif // WIDGETSERVERCONTENT_H
