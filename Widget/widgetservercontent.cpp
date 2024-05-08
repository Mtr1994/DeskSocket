#include "widgetservercontent.h"
#include "ui_widgetservercontent.h"
#include "Network/networkobject.h"
#include "Public/appsignal.h"
#include "Log/logger.h"

#include <QDateTime>

WidgetServerContent::WidgetServerContent(NetworkObject *netObj, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetServerContent), mNetworkObject(netObj)
{
    ui->setupUi(this);

    init();
}

WidgetServerContent::~WidgetServerContent()
{
    delete ui;
}

void WidgetServerContent::init()
{
    connect(AppSignal::getInstance(), &AppSignal::sgl_update_network_object, this, &WidgetServerContent::slot_update_network_object);
    connect(AppSignal::getInstance(), &AppSignal::sgl_recreate_network_object, this, &WidgetServerContent::slot_recreate_network_object);

    NetworkObjectDetail detail = mNetworkObject->getObjectDetail();
    //ui->lbServerIconType->setText(detail.type == Tcp_Server ? "T" : "U");

    ui->lbServerType->setText(detail.type == Tcp_Server ? "TCP" : "UDP");
    ui->lbServerCreateTime->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    ui->lbServerAddress->setText("127.0.0.1");
    ui->lbServerPort->setText("0000");

    ui->lbClientNumber->setText("0");

    QString statusText = getServerStatusText(detail.status);
    ui->lbServerStatus->setText(statusText);
}

QString WidgetServerContent::getServerStatusText(int status)
{
    switch (status)
    {
        case 0:
            return "<span style=\"color:#1890fe\">&nbsp;准备监听</span>";
        case 1:
            return "<span style=\"color:#52c41a\">&nbsp;正在监听</span>";
        case 2:
            return "<span style=\"color:#1890fe\">&nbsp;正在停止</span>";
        case 3:
            return "<span style=\"color:#f5222d\">&nbsp;关闭服务</span>";
        default:
            return "无效状态";
            break;
    }
}

void WidgetServerContent::slot_update_network_object(const QString &token)
{
    NetworkObjectDetail detail = mNetworkObject->getObjectDetail();
    if (token != detail.token) return;

    //ui->lbServerIconType->setText(detail.type == Tcp_Server ? "T" : "U");
    ui->lbServerType->setText(detail.type == Tcp_Server ? "TCP" : "UDP");

    ui->lbClientNumber->setText(QString::number(detail.clientNumber));
    ui->lbServerAddress->setText(detail.localAddress);
    ui->lbServerPort->setText(QString::number(detail.localPort));

    QString statusText = getServerStatusText(detail.status);
    ui->lbServerStatus->setText(statusText);
}

void WidgetServerContent::slot_recreate_network_object(const QString &token, const QString &address, uint16_t port)
{
    NetworkObjectDetail detail = mNetworkObject->getObjectDetail();
    if (detail.token != token) return;
    LOG_DEBUG("new address is {} new port is {}", address.toStdString(), port);
    ui->lbServerAddress->setText(address);
    ui->lbServerPort->setText(QString::number(port));
}
