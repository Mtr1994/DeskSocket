#ifndef WIDGETTABCONTENT_H
#define WIDGETTABCONTENT_H

#include <QWidget>
#include <QStandardItem>
#include <QByteArray>
#include <QList>

#include "Public/defines.h"

namespace Ui {
class WidgetTabContent;
}

class WidgetTabContent : public QWidget
{
    Q_OBJECT
public:
    enum DataEnum {RECV_DATA = 1, SEND_DATA, SYSTEM_DATA};
    explicit WidgetTabContent(const QString &flag, const QString &key, const QString &ip_4, uint16_t port, uint64_t dwconnid, QWidget *parent = nullptr);
    ~WidgetTabContent();

    void init();

    void appendError(const QString& data);
    void appendData(const QString& data, int type = 1);

    void applySendResult(int length);

    std::string getCurrentCodeTypeName();

signals:
    void sgl_client_operation(int operation, const ClientInfo& info);

private slots:
    void on_btnSend_clicked();

private:
    Ui::WidgetTabContent *ui;

    // 内容标志（改 四元组描述的是 被动 TCP、主动 TCP或者是 UDP）
    QString mContentFlag;

    // 服务端信息
    QString mServerKey;

    // 连接信息
    QString mClientAddress;
    uint16_t mClientPort;
    uint64_t mClientID;

    // tab 所对应的套接字
    int mSocketptr = 0;
};

#endif // WIDGETTABCONTENT_H
