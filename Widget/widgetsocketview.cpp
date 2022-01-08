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
#include "Dialog/dialogtcpserverargs.h"
#include "Dialog/dialogtcpclientargs.h"
#include "Public/appsignal.h"
#include "Manager/servermanager.h"
#include "Manager/clientmanager.h"

//test
#include <QDebug>

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

    // TCP 服务信号
    connect(AppSignal::getInstance(), &AppSignal::sgl_tcp_server_status_change, this, &WidgetSocketView::slot_tcp_server_status_change);
    connect(AppSignal::getInstance(), &AppSignal::sgl_recv_new_slave_tcp_client, this, &WidgetSocketView::slot_recv_new_slave_tcp_client);
    connect(AppSignal::getInstance(), &AppSignal::sgl_close_slave_tcp_client_result, this, &WidgetSocketView::slot_close_slave_tcp_client_result);
    connect(AppSignal::getInstance(), &AppSignal::sgl_tcp_client_connected, this, &WidgetSocketView::slot_tcp_client_connected);
    connect(AppSignal::getInstance(), &AppSignal::sgl_tcp_client_closed, this, &WidgetSocketView::slot_tcp_client_closed);

    connect(AppSignal::getInstance(), &AppSignal::sgl_delete_tcp_client_finish, this, &WidgetSocketView::slot_delete_tcp_client_finish);
    connect(AppSignal::getInstance(), &AppSignal::sgl_delete_tcp_server_finish, this, &WidgetSocketView::slot_delete_tcp_server_finish);
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

void WidgetSocketView::slot_server_operation(int operation, const ServerInfo &info)
{
    if (operation == Server_Add)
    {
        LOG_DEBUG("add server");
        QString key = QString("%1:%2").arg(info.ip).arg(info.port);
        QStandardItem *item = new QStandardItem(QIcon(":/resources/image/public/server.png"), info.key);
        item->setData(Tcp_Server, Qt::UserRole + 1);
        item->setData(info.name, Qt::UserRole + 2);
        item->setData(info.ip, Qt::UserRole + 3);
        item->setData(info.port, Qt::UserRole + 4);
        mModelSockets->appendRow(item);
    }
    else if (operation == Server_Close)
    {
         QModelIndexList list = mModelSockets->match(mModelSockets->index(0, 0), Qt::DisplayRole, info.key, 1, Qt::MatchExactly | Qt::MatchRecursive);
         if (list.isEmpty()) return;
         mModelSockets->removeRow(list.at(0).row());
    }
}

void WidgetSocketView::slot_client_operation(int operation, const ClientInfo &info)
{
    if (operation == Client_Add)
    {
        if (info.type == Tcp_Client_Slave)
        {
            QModelIndexList list = mModelSockets->match(mModelSockets->index(0, 0), Qt::DisplayRole, info.socketkey, 1, Qt::MatchExactly | Qt::MatchRecursive);
            if (list.isEmpty())
            {
                LOG_DEBUG("add client slave");
                QStandardItem *item = new QStandardItem(QIcon(":/resources/image/public/client.png"), info.socketkey);
                item->setData(info.type, Qt::UserRole + 1);
                item->setData(info.serverkey, Qt::UserRole + 2);
                item->setData(info.socketkey, Qt::UserRole + 3);
                item->setData(info.socketDescriptor, Qt::UserRole + 4);

                list = mModelSockets->match(mModelSockets->index(0, 0), Qt::DisplayRole, info.serverkey, 1, Qt::MatchExactly | Qt::MatchRecursive);

                if (list.isEmpty())
                {
                    delete item;
                    return;
                }
                QStandardItem *parentItem = mModelSockets->itemFromIndex(list.at(0));
                if (nullptr == parentItem)
                {
                    delete item;
                    return;
                }
                else
                {
                    parentItem->appendRow(item);
                    ui->socketView->setCurrentIndex(item->index());
                }
            }
        }
        else if (info.type == Tcp_Client)
        {
            LOG_DEBUG("add client");
            QStandardItem *item = new QStandardItem(QIcon(":/resources/image/public/client.png"), info.socketkey);
            item->setData(info.type, Qt::UserRole + 1);
            item->setData(info.serverkey, Qt::UserRole + 2);
            item->setData(info.socketkey, Qt::UserRole + 3);
            item->setData(info.socketDescriptor, Qt::UserRole + 4);

            mModelSockets->appendRow(item);
        }
    }
    else if (operation == Client_Close)
    {
        QModelIndexList list = mModelSockets->match(mModelSockets->index(0, 0), Qt::DisplayRole, info.socketkey, 1, Qt::MatchExactly | Qt::MatchRecursive);
        if (list.isEmpty()) return;

        QModelIndex index = list.first();
        mModelSockets->removeRow(index.row(), index.parent());
    }
}

void WidgetSocketView::slot_open_add_tcp_server_dialog()
{
    DialogTcpServerArgs dialog(this);
    dialog.exec();

    if (dialog.result() == 0) return;

    // 添加本地记录 (默认正在启动状态)
    QString ip_4 = dialog.getAddress();
    uint32_t port = dialog.getPort().toUInt();
    QString key = QString("%1:%2").arg(ip_4, QString::number(port));

    // 查找，已存在则不再添加
    QString mark = "TCPSERVER" + key;
    QModelIndexList list = mModelSockets->match(mModelSockets->index(0, 0), Qt::UserRole + 1, mark, 1, Qt::MatchRecursive | Qt::MatchExactly);
    if (!list.empty())
    {
        // 已存在
        return;
    }

    QStandardItem *item = new QStandardItem(QIcon(":/resources/image/public/server.png"), key);
    item->setData(mark, Qt::UserRole + 1);
    item->setData(Tcp_Server_StartUp, Qt::UserRole + 2);
    mModelSockets->appendRow(item);

    emit AppSignal::getInstance()->sgl_add_new_tcp_server(ip_4, port);
}

void WidgetSocketView::slot_open_add_tcp_client_dialog()
{
    DialogTcpClientArgs dialog(this);
    dialog.exec();

    if (dialog.result() == 0) return;

    // 添加本地记录 (默认正在链接状态)
    QString ip_4 = dialog.getAddress();
    uint32_t port = dialog.getPort().toUInt();
    QString key = QString("%1:%2").arg(ip_4, QString::number(port));

    // 查找，已存在则不再添加
    QString mark = "TCPCLIENT" + key;
    QModelIndexList list = mModelSockets->match(mModelSockets->index(0, 0), Qt::UserRole + 1, mark, 1, Qt::MatchRecursive | Qt::MatchExactly);
    if (!list.empty())
    {
        // 已存在
        return;
    }

    QStandardItem *item = new QStandardItem(QIcon(":/resources/image/public/client.png"), key);
    item->setData(mark, Qt::UserRole + 1);
    item->setData(Tcp_Client_Connecting, Qt::UserRole + 2);
    item->setData(key, Qt::UserRole + 4);
    mModelSockets->appendRow(item);

    emit AppSignal::getInstance()->sgl_add_new_tcp_client(ip_4, port);
}

void WidgetSocketView::slot_tcp_server_status_change(const QString &ip_4, uint16_t port, int status)
{
    qDebug() << "ip_4" << ip_4 << " port " << port << " status " << status;
    QString mark = QString("TCPSERVER%1:%2").arg(ip_4, QString::number(port));
    QModelIndexList list = mModelSockets->match(mModelSockets->index(0, 0), Qt::UserRole + 1, mark, 1, Qt::MatchRecursive | Qt::MatchExactly);
    if (list.empty())
    {
        qDebug() << "找不到服务，改变状态失败" << mark;
    }
    else
    {
        for (auto index : list)
        {
            if (!index.isValid()) continue;
            QStandardItem *item = mModelSockets->itemFromIndex(index);
            if (nullptr != item)
            {
                qDebug() << "找到目标  TCP  服务" << index.data(Qt::DisplayRole).toString();
                item->setData(status, Qt::UserRole + 2);
                QString key = QString("%1:%2").arg(ip_4, QString::number(port));
                item->setData(key, Qt::UserRole + 4);
                break;
            }
        }
    }
}

void WidgetSocketView::slot_recv_new_slave_tcp_client(const QString &key, const QString &ip_4, uint16_t port, uint64_t dwconnid)
{
    QModelIndexList list = mModelSockets->match(mModelSockets->index(0, 0), Qt::DisplayRole, key, -1, Qt::MatchRecursive | Qt::MatchExactly);
    if (list.empty())
    {
        qDebug() << "找不到";
    }
    else
    {
        for (auto index : list)
        {
            if (!index.isValid()) continue;
            QStandardItem *item = mModelSockets->itemFromIndex(index);
            if (nullptr != item)
            {
                QString childKey = QString("%1:%2").arg(ip_4, QString::number(port));
                QStandardItem *child = new QStandardItem(QIcon(":/resources/image/public/client.png"), childKey);
                child->setData("TCPSLAVECLIENT" + childKey, Qt::UserRole + 1);
                child->setData(Tcp_Client_Connected, Qt::UserRole + 2);

                // 四元组，确定唯一 socket
                QString socketKey = QString("%1*%2*%3*%4").arg(key, ip_4, QString::number(port), QString::number(dwconnid));
                qDebug() << "add  socket key " << socketKey;
                child->setData(socketKey, Qt::UserRole + 3);
                child->setData(key, Qt::UserRole + 4);
                child->setData(dwconnid, Qt::UserRole + 5);
                item->appendRow(child);

                // 通知页面新增 Tab 页
                emit AppSignal::getInstance()->sgl_add_tcp_tab_page("TCPSLAVECLIENT", key, ip_4, port, dwconnid);
                break;
            }
        }
    }
}

void WidgetSocketView::slot_close_slave_tcp_client_result(const QString &socketkey)
{
    QModelIndexList list = mModelSockets->match(mModelSockets->index(0, 0), Qt::UserRole + 3, socketkey, 1, Qt::MatchRecursive | Qt::MatchExactly);
    if (list.empty())
    {
        qDebug() << "找不到指定的 TCP 连接";
    }
    else
    {
        QModelIndex index = list.at(0);
        if (!index.isValid()) return;

        mModelSockets->removeRow(index.row(), index.parent());
    }
}

void WidgetSocketView::slot_tcp_client_connected(const QString &key, const QString &ip_4, uint16_t port, uint64_t dwconnid)
{
    QString mask = "TCPCLIENT" + key;
    QModelIndexList list = mModelSockets->match(mModelSockets->index(0, 0), Qt::UserRole + 1, mask, 1, Qt::MatchRecursive | Qt::MatchExactly);
    if (list.empty())
    {
        qDebug() << "找不到";
    }
    else
    {
        qDebug() << "修改状态（待完成）";
        // 通知页面新增 Tab 页
        QModelIndex index = list.at(0);
        if (!index.isValid()) return;
        QStandardItem *item = mModelSockets->itemFromIndex(index);
        if (nullptr != item)
        {
            item->setData(Tcp_Client_Connected, Qt::UserRole + 2);

            // 四元组，确定唯一 socket
            QString socketKey = QString("%1*%2*%3*%4").arg(key, ip_4, QString::number(port), QString::number(dwconnid));
            qDebug() << "socketKey 2" << socketKey;
            item->setData(socketKey, Qt::UserRole + 3);
            item->setData(dwconnid, Qt::UserRole + 5);

            emit AppSignal::getInstance()->sgl_add_tcp_tab_page("TCPCLIENT", key, ip_4, port, dwconnid);
        }
    }
}

void WidgetSocketView::slot_tcp_client_closed(const QString &socketkey)
{
    QStringList listSocketInfo = socketkey.split('*');
    if (listSocketInfo.length() != 4)
    {
        qDebug() << "报告错误";
        return;
    }
    QString mark = "TCPCLIENT" + listSocketInfo.at(0);
    qDebug() << "slot_tcp_client_closed " << mark;
    QModelIndexList list = mModelSockets->match(mModelSockets->index(0, 0), Qt::UserRole + 1, mark, 1, Qt::MatchRecursive | Qt::MatchExactly);
    if (list.empty())
    {
        qDebug() << "找不到指定的 TCP (主动)连接";
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
            item->setData(Tcp_Client_Closed, Qt::UserRole + 2);
        }
    }
}

void WidgetSocketView::slot_delete_tcp_client_finish(const QString &serverkey, bool status)
{
    if (!status)
    {
        qDebug() << "报告删除失败";
        return;
    }
    QString mark = "TCPCLIENT" + serverkey;
    QModelIndexList list = mModelSockets->match(mModelSockets->index(0, 0), Qt::UserRole + 1, mark, 1, Qt::MatchRecursive | Qt::MatchExactly);
    if (list.empty())
    {
        qDebug() << "找不到指定的 TCP (主动)连接";
    }
    else
    {
        QModelIndex index = list.at(0);
        if (!index.isValid()) return;
        mModelSockets->removeRow(index.row(), index.parent());
    }
}

void WidgetSocketView::slot_delete_tcp_server_finish(const QString &serverkey)
{
    QModelIndexList list = mModelSockets->match(mModelSockets->index(0, 0), Qt::UserRole + 4, serverkey, 1, Qt::MatchRecursive | Qt::MatchExactly);
    if (list.empty())
    {
        qDebug() << "找不到指定的 TCP 服务 " << serverkey;
    }
    else
    {
        QModelIndex index = list.at(0);
        if (!index.isValid()) return;

        mModelSockets->removeRow(index.row(), index.parent());
    }
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

    qDebug() << "mask " << mark;
    if (mark.contains("TCPSERVER"))
    {
        QMenu menu(this);
        int status = item->data(Qt::UserRole + 2).toUInt();
        QString key = item->data(Qt::UserRole + 4).toString();

        QAction actionStart("开始监听");
        actionStart.setEnabled(status == Tcp_Server_Shutdown);
        connect(&actionStart, &QAction::triggered, [key]() {
            emit AppSignal::getInstance()->sgl_start_tcp_server(key);
        });
        QAction actionStop("停止监听");
        actionStop.setDisabled(status == Tcp_Server_Shutdown);
        connect(&actionStop, &QAction::triggered, [key]() {
            emit AppSignal::getInstance()->sgl_stop_tcp_server(key);
        });
        QAction actionDeleteServer("删除服务");
        connect(&actionDeleteServer, &QAction::triggered, [key]() {
            emit AppSignal::getInstance()->sgl_delete_tcp_server(key);
        });
        QAction actionDeleteConnect("删除所有连接");
        actionDeleteConnect.setEnabled(status == Tcp_Server_On);
        connect(&actionDeleteConnect, &QAction::triggered, [key]() {
           emit AppSignal::getInstance()->sgl_close_all_slave_tcp_client(key);
        });

        menu.addAction(&actionStart);
        menu.addAction(&actionStop);
        menu.addAction(&actionDeleteServer);
        menu.addAction(&actionDeleteConnect);

        menu.exec(QCursor::pos());
    }
    else if (mark.contains("TCPSLAVECLIENT"))
    {
        QMenu menu(this);
        QAction actionDeleteClient("断开连接");
        QString serverKey = item->data(Qt::UserRole + 4).toString();
        uint64_t dwconnid = item->data(Qt::UserRole + 5).toULongLong();
        connect(&actionDeleteClient, &QAction::triggered, [serverKey, dwconnid]() {
            emit AppSignal::getInstance()->sgl_delete_slave_tcp_client(serverKey, dwconnid);
        });

        menu.addAction(&actionDeleteClient);
        menu.exec(QCursor::pos());
    }
    else if (mark.contains("TCPCLIENT"))
    {
        QMenu menu(this);
        int status = item->data(Qt::UserRole + 2).toUInt();
        QString serverKey = item->data(Qt::UserRole + 4).toString();

        QAction actionReconnect("重新连接");
        qDebug() << "status " << status;
        actionReconnect.setEnabled(status == Tcp_Client_Closed);
        connect(&actionReconnect, &QAction::triggered, [serverKey]() {
            emit AppSignal::getInstance()->sgl_connect_tcp_client(serverKey);
        });
        menu.addAction(&actionReconnect);

        QAction actionDisconnect("断开连接");
        actionDisconnect.setDisabled(status == Tcp_Client_Closed);
        connect(&actionDisconnect, &QAction::triggered, [serverKey]() {
            emit AppSignal::getInstance()->sgl_disconnect_tcp_client(serverKey);
        });
        menu.addAction(&actionDisconnect);

        QAction actionDeleteClient("删除连接");
        connect(&actionDeleteClient, &QAction::triggered, [serverKey]() {
            emit AppSignal::getInstance()->sgl_delete_tcp_client(serverKey);
        });
        menu.addAction(&actionDeleteClient);

        menu.exec(QCursor::pos());
    }
}
