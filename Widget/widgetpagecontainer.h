#ifndef WIDGETPAGECONTAINER_H
#define WIDGETPAGECONTAINER_H

#include <QWidget>
#include <QList>

#include "Network/networkobject.h"

namespace Ui {
class WidgetPageContainer;
}

class WidgetPageContainer : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetPageContainer(QWidget *parent = nullptr);
    ~WidgetPageContainer();

    void init();

    void createCustomContent(const QString& name);

private slots:
    void slot_create_network_object_finish(NetworkObject* netObj);
    void slot_recv_new_network_object(NetworkObject* netObj, uint16_t dwConnID);
    void slot_current_network_object_change(const QString& token);
    void slot_delete_network_object(const QString& token);

private:
    Ui::WidgetPageContainer *ui;

    QList<QString> mListToken;
};

#endif // WIDGETPAGECONTAINER_H
