#ifndef WIDGETTABITEMS_H
#define WIDGETTABITEMS_H

#include <QWidget>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QString>
#include <QMap>

#include "Widget/widgettabcontent.h"
#include "Public/defines.h"

namespace Ui {
class WidgetTabPages;
}

class WidgetTabPages : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetTabPages(QWidget *parent = nullptr);
    ~WidgetTabPages();

    void init();

    int getTabCount();

public:
    void removeTab(int socketptr);
    void setCurrentIndex(int index);

    void appentData(int socketptr, const QByteArray& data);

signals:
    void sgl_client_operation(int operation, const ClientInfo& info);

public slots:
    void slot_add_tcp_tab_page(const QString &flag, const QString &key, const QString &ip_4, uint16_t port, uint64_t dwconnid);
    void slot_thread_recv_slave_tcp_client_data(const QString &key, const QString &ip_4, uint16_t port, uint64_t dwconnid, const QByteArray &data);
    void slot_current_socket_index_change(const QString &socketkey);
    void slot_slave_tcp_client_sent_data_result(const QString &contentKey, bool status, uint32_t length, const QString &error);
    void slot_close_slave_tcp_client_result(const QString &socketkey);
    void slot_thread_recv_tcp_client_data(const QString &key, const QString &ip_4, uint16_t port, uint64_t dwconnid, const QByteArray &data);
    void slot_tcp_client_sent_data_result(const QString &contentKey, bool status, uint32_t length, const QString &error);
    void sgl_tcp_client_closed(const QString &socketkey);

private:
    Ui::WidgetTabPages *ui;

    QMap<QString ,WidgetTabContent*> mMapContent;
};

#endif // WIDGETTABITEMS_H
