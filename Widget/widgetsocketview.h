#ifndef WIDGETSOCKETVIEW_H
#define WIDGETSOCKETVIEW_H

#include <QWidget>
#include <QString>
#include <QModelIndex>
#include <QStandardItemModel>
#include <QStandardItem>

#include "Manager/servermanager.h"
#include "Manager/clientmanager.h"

namespace Ui {
class WidgetSocketView;
}

class WidgetSocketView : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetSocketView(QWidget *parent = nullptr);
    ~WidgetSocketView();

    void init();

    void deleteItem(int index);

    bool contains(int index);

    void setItemExpand(const QStandardItem* item);

    void setCurrentIndex(const QModelIndex& index);

signals:
    void sgl_current_socket_change(int index);
    void sgl_server_operation(int operation, const ServerInfo& info);
    void sgl_client_operation(int operation, const ClientInfo& info);

public slots:
    void slot_server_operation(int operation, const ServerInfo& info);
    void slot_client_operation(int operation, const ClientInfo& info);

private slots:
    void on_socketView_customContextMenuRequested(const QPoint &pos);

    // 自定义信号
    void slot_open_add_tcp_server_dialog();
    void slot_open_add_tcp_client_dialog();
    void slot_tcp_server_status_change(const QString &ip_4, uint16_t port, int status);
    void slot_recv_new_slave_tcp_client(const QString &key, const QString &ip_4, uint16_t port, uint64_t socketptr);
    void slot_close_slave_tcp_client_result(const QString &socketkey);

    // 主动建立的客户端信号
    void slot_tcp_client_connected(const QString &key, const QString &ip_4, uint16_t port, uint64_t dwconnid);
    void slot_tcp_client_closed(const QString &socketkey);

    void slot_delete_tcp_client_finish(const QString &serverkey, bool status);

    void slot_delete_tcp_server_finish(const QString &serverkey);


private:
    void slot_current_change(const QModelIndex &current, const QModelIndex &previous);

private:
    Ui::WidgetSocketView *ui;
    QStandardItemModel *mModelSockets = nullptr;
};

#endif // WIDGETSOCKETVIEW_H
