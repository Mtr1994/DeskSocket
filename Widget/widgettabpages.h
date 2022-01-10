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
    void setCurrentIndex(int index);

public slots:
    void slot_add_tab_page(uint16_t protocol, const QString &flag, const QString &key, const QString &ip_4, uint16_t port, uint64_t dwconnid);
    void slot_thread_recv_slave_client_data(const QString &key, const QString &ip_4, uint16_t port, uint64_t dwconnid, const QByteArray &data);
    void slot_current_socket_index_change(const QString &socketkey);
    void slot_slave_client_sent_data_result(const QString &contentKey, bool status, uint32_t length, const QString &error);
    void slot_close_slave_client_result(const QString &socketkey);
    void slot_thread_recv_client_data(const QString &key, const QString &ip_4, uint16_t port, uint64_t dwconnid, const QByteArray &data);
    void slot_client_sent_data_result(const QString &contentKey, bool status, uint32_t length, const QString &error);
    void slot_client_closed(const QString &socketkey);

private:
    Ui::WidgetTabPages *ui;

    QMap<QString ,WidgetTabContent*> mMapContent;
};

#endif // WIDGETTABITEMS_H
