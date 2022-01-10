#include "widgetsocketview.h"
#include "ui_widgetsocketview.h"

#include <QModelIndexList>
#include <QItemSelectionModel>
#include <QMenu>
#include <QAction>

#include "Public/treeitemdelegate.h"
#include "Public/defines.h"
#include "Dialog/dialognote.h"
#include "Log/logger.h"
#include "Dialog/dialogserverargs.h"
#include "Public/appsignal.h"
#include "Manager/servermanager.h"
#include "Manager/clientmanager.h"
#include "Log/logger.h"

using namespace mtr;

WidgetSocketView::WidgetSocketView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetSocketView)
{
    ui->setupUi(this);

    //
    init();
}

WidgetSocketView::~WidgetSocketView()
{
    delete ui;
    if (nullptr != mModelSockets) delete mModelSockets;
}

void WidgetSocketView::init()
{
    mModelSockets = new QStandardItemModel(this);
    ui->socketView->setItemDelegate(new TreeItemDelegate);
    ui->socketView->setModel(mModelSockets);

    ServerManager::getInstance()->init();
    ClientManager::getInstance()->init();

    // 信号管理
    connect(ui->socketView->selectionModel(), &QItemSelectionModel::currentRowChanged, this, &WidgetSocketView::slot_current_change);
    connect(AppSignal::getInstance(), &AppSignal::sgl_open_add_tcp_server_dialog, this, &WidgetSocketView::slot_open_add_tcp_server_dialog);
    connect(AppSignal::getInstance(), &AppSignal::sgl_open_add_tcp_client_dialog, this, &WidgetSocketView::slot_open_add_tcp_client_dialog);
    connect(AppSignal::getInstance(), &AppSignal::sgl_open_add_udp_server_dialog, this, &WidgetSocketView::slot_open_add_udp_server_dialog);
    connect(AppSignal::getInstance(), &AppSignal::sgl_open_add_udp_client_dialog, this, &WidgetSocketView::slot_open_add_udp_client_dialog);


    // TCP 服务信号
    connect(AppSignal::getInstance(), &AppSignal::sgl_server_status_change, this, &WidgetSocketView::slot_server_status_change);
    connect(AppSignal::getInstance(), &AppSignal::sgl_recv_new_slave_client, this, &WidgetSocketView::slot_recv_new_slave_client);
    connect(AppSignal::getInstance(), &AppSignal::sgl_close_slave_client_result, this, &WidgetSocketView::slot_close_slave_client_result);
    connect(AppSignal::getInstance(), &AppSignal::sgl_client_connected, this, &WidgetSocketView::slot_client_connected);
    connect(AppSignal::getInstance(), &AppSignal::sgl_client_closed, this, &WidgetSocketView::slot_client_closed);

    connect(AppSignal::getInstance(), &AppSignal::sgl_delete_client_finish, this, &WidgetSocketView::slot_delete_client_finish);
    connect(AppSignal::getInstance(), &AppSignal::sgl_delete_server_finish, this, &WidgetSocketView::slot_delete_server_finish);

}

void WidgetSocketView::deleteItem(int index)
{
    if (!contains(index)) return;
    QModelIndexList list = mModelSockets->match(mModelSockets->index(0, 0), Qt::UserRole + 1, index, 1, Qt::MatchExactly | Qt::MatchRecursive);
    int row = list.at(0).row();
    mModelSockets->removeRow(row);
}

bool WidgetSocketView::contains(int index)
{
    QModelIndexList list = mModelSockets->match(mModelSockets->index(0, 0), Qt::UserRole + 1, index, 1, Qt::MatchExactly | Qt::MatchRecursive);
    return list.length() > 0;
}

void WidgetSocketView::setItemExpand(const QStandardItem *item)
{
    if (nullptr == item) return;
    ui->socketView->expand(item->index());
}

void WidgetSocketView::setCurrentIndex(const QModelIndex &index)
{
    if (!index.isValid()) return;
    ui->socketView->setCurrentIndex(index);
}

void WidgetSocketView::slot_open_add_tcp_server_dialog()
{
    DialogServerArgs dialog("TCP 服务端参数", this);
    dialog.exec();

    if (dialog.result() == 0) return;

    // 添加本地记录 (默认正在启动状态)
    QString ip_4 = dialog.getAddress();
    uint32_t port = dialog.getPort().toUInt();
    QString key = QString("%1:%2").arg(ip_4, QString::number(port));

    // 查找，已存在则不再添加
    QString mark = "TCPSERVER:" + key;
    QModelIndexList list = mModelSockets->match(mModelSockets->index(0, 0), Qt::UserRole + 1, mark, 1, Qt::MatchRecursive | Qt::MatchExactly);
    if (!list.empty())
    {
        // 已存在
        return;
    }

    QStandardItem *item = new QStandardItem(QIcon(":/resources/image/public/server.png"), key);
    item->setData(mark, Qt::UserRole + 1);
    item->setData(Server_Starting, Qt::UserRole + 2);
    mModelSockets->appendRow(item);

    emit AppSignal::getInstance()->sgl_add_new_server(TCP, ip_4, port);
}

void WidgetSocketView::slot_open_add_tcp_client_dialog()
{
    DialogServerArgs dialog("TCP 客户端参数", this);
    dialog.exec();

    if (dialog.result() == 0) return;

    // 添加本地记录 (默认正在链接状态)
    QString ip_4 = dialog.getAddress();
    uint32_t port = dialog.getPort().toUInt();
    QString key = QString("%1:%2").arg(ip_4, QString::number(port));

    // 查找，已存在则不再添加
    QString mark = "TCPCLIENT:" + key;
    QModelIndexList list = mModelSockets->match(mModelSockets->index(0, 0), Qt::UserRole + 1, mark, 1, Qt::MatchRecursive | Qt::MatchExactly);
    if (!list.empty())
    {
        // 已存在
        return;
    }

    QStandardItem *item = new QStandardItem(QIcon(":/resources/image/public/client.png"), key);
    item->setData(mark, Qt::UserRole + 1);
    item->setData(Client_Connecting, Qt::UserRole + 2);
    item->setData(key, Qt::UserRole + 4);
    mModelSockets->appendRow(item);

    emit AppSignal::getInstance()->sgl_add_new_client(TCP, ip_4, port);
}

void WidgetSocketView::slot_server_status_change(uint16_t protocol, const QString &ip_4, uint16_t port, int status)
{
    QString mark = ((protocol == TCP) ? "TCP" : "UDP") + QString("SERVER:%1:%2").arg(ip_4, QString::number(port));
    QModelIndexList list = mModelSockets->match(mModelSockets->index(0, 0), Qt::UserRole + 1, mark, 1, Qt::MatchRecursive | Qt::MatchExactly);
    if (list.empty())
    {
        LOG_INFO(QString("找不到服务，改变状态失败 %1").arg(mark).toStdString().data());
        return;
    }
    for (auto index : list)
    {
        if (!index.isValid()) continue;
        QStandardItem *item = mModelSockets->itemFromIndex(index);
        if (nullptr != item)
        {
            item->setData(status, Qt::UserRole + 2);
            break;
        }
    }
}

void WidgetSocketView::slot_recv_new_slave_client(uint16_t protocol, const QString &serverkey, const QString &ip_4, uint16_t port, uint64_t dwconnid)
{
    QModelIndexList list = mModelSockets->match(mModelSockets->index(0, 0), Qt::UserRole + 1, serverkey, 1, Qt::MatchRecursive | Qt::MatchExactly);
    if (list.empty())
    {
        LOG_INFO(QString("找不到所属的服务 %1").arg(serverkey).toStdString().data());
        return;
    }

    for (auto index : list)
    {
        if (!index.isValid()) continue;
        QStandardItem *item = mModelSockets->itemFromIndex(index);
        if (nullptr != item)
        {
            QString childName = QString("%1:%2").arg(ip_4, QString::number(port));
            QStandardItem *child = new QStandardItem(QIcon(":/resources/image/public/client.png"), childName);
            child->setData(QString("%1%2:%3").arg(((protocol == TCP) ? "TCP" : "UDP"), "SLAVECLIENT", childName), Qt::UserRole + 1);
            child->setData(Client_Connected, Qt::UserRole + 2);

            // 四元组，确定唯一 socket
            QString socketKey = QString("%1*%2*%3*%4").arg(serverkey, ip_4, QString::number(port), QString::number(dwconnid));
            child->setData(socketKey, Qt::UserRole + 3);
            child->setData(serverkey, Qt::UserRole + 4);
            child->setData(dwconnid, Qt::UserRole + 5);
            item->appendRow(child);

            // 通知页面新增 Tab 页
            emit AppSignal::getInstance()->sgl_add_tab_page(protocol, "SLAVECLIENT", serverkey, ip_4, port, dwconnid);
            break;
        }
    }
}

void WidgetSocketView::slot_close_slave_client_result(const QString &socketkey)
{
    QModelIndexList list = mModelSockets->match(mModelSockets->index(0, 0), Qt::UserRole + 3, socketkey, 1, Qt::MatchRecursive | Qt::MatchExactly);
    if (list.empty())
    {
        LOG_INFO(QString("找不到指定的连接 %1").arg(socketkey).toStdString().data());
        return;
    }

    QModelIndex index = list.at(0);
    if (!index.isValid()) return;

    mModelSockets->removeRow(index.row(), index.parent());
}

void WidgetSocketView::slot_client_connected(uint16_t protocol, const QString &serverkey, const QString &ip_4, uint16_t port, uint64_t dwconnid)
{
    QString mask = serverkey;
    QModelIndexList list = mModelSockets->match(mModelSockets->index(0, 0), Qt::UserRole + 1, mask, 1, Qt::MatchRecursive | Qt::MatchExactly);
    if (list.empty())
    {
        LOG_INFO(QString("找不到指定服务 %1").arg(serverkey).toStdString().data());
        return;
    }

    // 通知页面新增 Tab 页
    QModelIndex index = list.at(0);
    if (!index.isValid()) return;
    QStandardItem *item = mModelSockets->itemFromIndex(index);
    if (nullptr != item)
    {
        item->setData(Client_Connected, Qt::UserRole + 2);

        // 四元组，确定唯一 socket
        QString socketKey = QString("%1*%2*%3*%4").arg(serverkey, ip_4, QString::number(port), QString::number(dwconnid));
        item->setData(socketKey, Qt::UserRole + 3);
        item->setData(serverkey, Qt::UserRole + 4);
        item->setData(dwconnid, Qt::UserRole + 5);

        emit AppSignal::getInstance()->sgl_add_tab_page(protocol, "CLIENT", serverkey, ip_4, port, dwconnid);
    }
}

void WidgetSocketView::slot_client_closed(const QString &socketkey)
{
    QStringList listSocketInfo = socketkey.split('*');
    if (listSocketInfo.length() != 4)
    {
        LOG_INFO("系统异常");
        return;
    }
    QString mark = listSocketInfo.at(0);
    QModelIndexList list = mModelSockets->match(mModelSockets->index(0, 0), Qt::UserRole + 1, mark, 1, Qt::MatchRecursive | Qt::MatchExactly);
    if (list.empty())
    {
        LOG_INFO(QString("找不到指定的(主动)连接 %1").arg(socketkey).toStdString().data());
    }
    else
    {
        QModelIndex index = list.at(0);
        if (!index.isValid()) return;
        // mModelSockets->removeRow(index.row(), index.parent());
        // 由于是主动连接，当通道关闭，应该只需要改变状态，不必删除该条目
        QStandardItem *item = mModelSockets->itemFromIndex(index);
        if (nullptr != item)
        {
            item->setData(Client_Closed, Qt::UserRole + 2);
        }
    }
}

void WidgetSocketView::slot_delete_client_finish(const QString &serverkey, bool status)
{
    if (!status)
    {
        LOG_INFO(QString("删除失败 %1").arg(serverkey).toStdString().data());
        return;
    }
    QString mark = serverkey;
    QModelIndexList list = mModelSockets->match(mModelSockets->index(0, 0), Qt::UserRole + 1, mark, 1, Qt::MatchRecursive | Qt::MatchExactly);
    if (list.empty())
    {
        LOG_INFO(QString("找不到指定的(主动)连接 %1").arg(serverkey).toStdString().data());
    }
    else
    {
        QModelIndex index = list.at(0);
        if (!index.isValid()) return;
        mModelSockets->removeRow(index.row(), index.parent());
    }
}

void WidgetSocketView::slot_delete_server_finish(const QString &serverkey)
{
    QModelIndexList list = mModelSockets->match(mModelSockets->index(0, 0), Qt::UserRole + 1, serverkey, 1, Qt::MatchRecursive | Qt::MatchExactly);
    if (list.empty())
    {
        LOG_INFO(QString("找不到指定的服务 %1").arg(serverkey).toStdString().data());
    }
    else
    {
        QModelIndex index = list.at(0);
        if (!index.isValid()) return;

        mModelSockets->removeRow(index.row(), index.parent());
    }
}

void WidgetSocketView::slot_open_add_udp_server_dialog()
{
    DialogServerArgs dialog("UDP 服务端参数", this);
    dialog.exec();

    if (dialog.result() == 0) return;

    // 添加本地记录 (默认正在启动状态)
    QString ip_4 = dialog.getAddress();
    uint32_t port = dialog.getPort().toUInt();
    QString key = QString("%1:%2").arg(ip_4, QString::number(port));

    // 查找，已存在则不再添加
    QString mark = "UDPSERVER:" + key;
    QModelIndexList list = mModelSockets->match(mModelSockets->index(0, 0), Qt::UserRole + 1, mark, 1, Qt::MatchRecursive | Qt::MatchExactly);
    if (!list.empty())
    {
        // 已存在
        return;
    }

    QStandardItem *item = new QStandardItem(QIcon(":/resources/image/public/server.png"), key);
    item->setData(mark, Qt::UserRole + 1);
    item->setData(Server_Starting, Qt::UserRole + 2);
    mModelSockets->appendRow(item);

    emit AppSignal::getInstance()->sgl_add_new_server(UDP, ip_4, port);
}

void WidgetSocketView::slot_open_add_udp_client_dialog()
{
    DialogServerArgs dialog("UDP 客户端参数", this);
    dialog.exec();

    if (dialog.result() == 0) return;

    // 添加本地记录 (默认正在链接状态)
    QString ipv4 = dialog.getAddress();
    uint32_t port = dialog.getPort().toUInt();
    QString key = QString("%1:%2").arg(ipv4, QString::number(port));

    // 查找，已存在则不再添加
    QString mark = "UDPCLIENT:" + key;
    QModelIndexList list = mModelSockets->match(mModelSockets->index(0, 0), Qt::UserRole + 1, mark, 1, Qt::MatchRecursive | Qt::MatchExactly);
    if (!list.empty())
    {
        // 已存在
        return;
    }

    QStandardItem *item = new QStandardItem(QIcon(":/resources/image/public/client.png"), key);
    item->setData(mark, Qt::UserRole + 1);
    item->setData(Client_Connected, Qt::UserRole + 2);
    mModelSockets->appendRow(item);

    emit AppSignal::getInstance()->sgl_add_new_client(UDP, ipv4, port);
}

void WidgetSocketView::slot_current_change(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous);
    if (!current.isValid()) return;
    QStandardItem *item = mModelSockets->itemFromIndex(current);
    if (nullptr == item) return;
    if (item->data(Qt::UserRole + 1).toString().contains("SERVER")) return;
    QString socketKey = item->data(Qt::UserRole + 3).toString();
    // 使用 四元组 定位 tab index
    emit AppSignal::getInstance()->sgl_current_socket_index_change(socketKey);
}

void WidgetSocketView::on_socketView_customContextMenuRequested(const QPoint &pos)
{
    // 渲染管道右键菜单
    QModelIndex index = ui->socketView->indexAt(pos);
    if (!index.isValid()) return;

    QStandardItem *item =mModelSockets->itemFromIndex(index);
    if (nullptr == item) return;
    QString mark = item->data(Qt::UserRole + 1).toString();

    if (mark.contains("SERVER"))
    {
        QMenu menu(this);
        int status = item->data(Qt::UserRole + 2).toUInt();
        QString serverKey = mark;
        QAction actionStart("开始监听");
        actionStart.setEnabled(status == Server_Shutdown);
        connect(&actionStart, &QAction::triggered, [serverKey]() {
            emit AppSignal::getInstance()->sgl_restart_server(serverKey);
        });
        QAction actionStop("停止监听");
        actionStop.setDisabled(status == Server_Shutdown);
        connect(&actionStop, &QAction::triggered, [serverKey]() {
            emit AppSignal::getInstance()->sgl_stop_server(serverKey);
        });
        QAction actiondelete_server("删除服务");
        connect(&actiondelete_server, &QAction::triggered, [serverKey]() {
            emit AppSignal::getInstance()->sgl_delete_server(serverKey);
        });
        QAction actionDeleteConnect("删除所有连接");
        actionDeleteConnect.setEnabled(status == Server_Started);
        connect(&actionDeleteConnect, &QAction::triggered, [serverKey]() {
           emit AppSignal::getInstance()->sgl_close_all_slave_client(serverKey);
        });

        menu.addAction(&actionStart);
        menu.addAction(&actionStop);
        menu.addAction(&actiondelete_server);
        menu.addAction(&actionDeleteConnect);

        menu.exec(QCursor::pos());
    }
    else if (mark.contains("SLAVECLIENT"))
    {
        QMenu menu(this);
        QAction actionDeleteClient("删除连接");
        QString serverKey = item->data(Qt::UserRole + 4).toString();
        uint64_t dwconnid = item->data(Qt::UserRole + 5).toULongLong();
        connect(&actionDeleteClient, &QAction::triggered, [serverKey, dwconnid]() {
            emit AppSignal::getInstance()->sgl_delete_slave_client(serverKey, dwconnid);
        });

        menu.addAction(&actionDeleteClient);
        menu.exec(QCursor::pos());
    }
    else if (mark.contains("TCPCLIENT"))
    {
        QMenu menu(this);
        int status = item->data(Qt::UserRole + 2).toUInt();
        QString serverKey = item->data(Qt::UserRole + 1).toString();

        QAction actionReconnect("重新连接");
        actionReconnect.setEnabled(status == Client_Closed);
        connect(&actionReconnect, &QAction::triggered, [serverKey]() {
            emit AppSignal::getInstance()->sgl_reconnect_client(serverKey);
        });
        menu.addAction(&actionReconnect);

        QAction actionDisconnect("断开连接");
        actionDisconnect.setDisabled(status == Client_Closed);
        connect(&actionDisconnect, &QAction::triggered, [serverKey]() {
            emit AppSignal::getInstance()->sgl_disconnect_client(serverKey);
        });
        menu.addAction(&actionDisconnect);

        QAction actionDeleteClient("删除连接");
        connect(&actionDeleteClient, &QAction::triggered, [serverKey]() {
            emit AppSignal::getInstance()->sgl_delete_client(serverKey);
        });
        menu.addAction(&actionDeleteClient);

        menu.exec(QCursor::pos());
    }
    else if (mark.contains("UDPCLIENT"))
    {
        QMenu menu(this);
        QAction actionDeleteClient("删除连接");
        QString serverKey = item->data(Qt::UserRole + 4).toString();
        connect(&actionDeleteClient, &QAction::triggered, [serverKey]() {
            emit AppSignal::getInstance()->sgl_delete_client(serverKey);
        });

        menu.addAction(&actionDeleteClient);
        menu.exec(QCursor::pos());
    }
}
