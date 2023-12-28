#ifndef WIDGETNETWROKOBJECTVIEW_H
#define WIDGETNETWROKOBJECTVIEW_H

#include <QWidget>
#include <QStandardItemModel>
#include <QMap>

#include "Network/networkobject.h"

namespace Ui {
class WidgetNetwrokObjectView;
}

class WidgetNetwrokObjectView : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetNetwrokObjectView(QWidget *parent = nullptr);
    ~WidgetNetwrokObjectView();

    void init();

private slots:
    void slot_view_custom_context_menu_requested(const QPoint &pos);
    void slot_create_network_object_finish(NetworkObject* netObj);
    void slot_recv_new_network_object(NetworkObject* netObj, uint16_t dwConnID);
    void slot_update_network_object(const QString &token);
    void slot_delete_network_object(const QString& token);
    void slot_current_network_object_change(const QModelIndex &current, const QModelIndex &previous);
    void slot_network_object_click(const QModelIndex &index);

private:
    Ui::WidgetNetwrokObjectView *ui;

    QStandardItemModel *mModelSockets = nullptr;

    QMap<QString, NetworkObject*> mMapNetworkObject;
};

#endif // WIDGETNETWROKOBJECTVIEW_H
