#ifndef WIDGETSOCKETCONTENT_H
#define WIDGETSOCKETCONTENT_H

#include <QWidget>
#include <QStandardItem>
#include <QByteArray>
#include <QList>
#include <QTimer>

#include "Public/defines.h"

namespace Ui {
class WidgetSocketContent;
}

class NetworkObject;
class WidgetSocketContent : public QWidget
{
    Q_OBJECT
public:
    enum DataEnum {RECV_DATA = 1, SEND_DATA, SYSTEM_DATA};
    explicit WidgetSocketContent(NetworkObject* netObj, QWidget *parent = nullptr);
    ~WidgetSocketContent();

    void init();

    void appendMessage(const QString& msg);

    void clearStatisticsNumber();

    void setDwConnID(int32_t id);

private slots:
    void slot_btn_send_click();

    void slot_tb_logs_costom_menu_request(const QPoint &pos);

    void slot_tb_sender_custom_menu_request(const QPoint &pos);

    void slot_btn_auto_send_click();

    void slot_auto_sent_trigger();

    void slot_update_network_object(const QString &token);

    void slot_recv_network_data(const QString &token, const std::string &data, uint16_t length);

    void slot_recreate_network_object(const QString &token, const QString &address, uint16_t port);

private:
    void updateStatistics();

private:
    Ui::WidgetSocketContent *ui;

    NetworkObject *mNetworkObject = nullptr;

    // 连接的标志
    int32_t mDwConnID = -1;

    // 发送的字节数量
    uint64_t mTotalSendBytes = 0;

    // 接收的字节数量
    uint64_t mTotalRecvBytes = 0;

    // 自动发送计时器
    QTimer mAutoSendTimer;
};

#endif // WIDGETSOCKETCONTENT_H
