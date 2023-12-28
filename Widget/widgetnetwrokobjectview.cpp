﻿#include "widgetnetwrokobjectview.h"
#include "ui_widgetnetwrokobjectview.h"
#include "Public/appsignal.h"
#include "Public/treeitemdelegate.h"

#include <QMenu>

// test
#include <QDebug>

WidgetNetwrokObjectView::WidgetNetwrokObjectView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetNetwrokObjectView)
{
    ui->setupUi(this);

    init();
}

WidgetNetwrokObjectView::~WidgetNetwrokObjectView()
{
    delete ui;
}

void WidgetNetwrokObjectView::init()
{
    mModelSockets = new QStandardItemModel(this);
    ui->tvNetworkObject->setItemDelegate(new TreeItemDelegate);
    ui->tvNetworkObject->setModel(mModelSockets);

    connect(ui->tvNetworkObject->selectionModel(), &QItemSelectionModel::currentRowChanged, this, &WidgetNetwrokObjectView::slot_current_network_object_change);
    connect(ui->tvNetworkObject, &QTreeView::clicked, this, &WidgetNetwrokObjectView::slot_network_object_click);

    connect(ui->tvNetworkObject, &QTreeView::customContextMenuRequested, this, &WidgetNetwrokObjectView::slot_view_custom_context_menu_requested);
    connect(AppSignal::getInstance(), &AppSignal::sgl_create_network_object_finish, this, &WidgetNetwrokObjectView::slot_create_network_object_finish);
    connect(AppSignal::getInstance(), &AppSignal::sgl_recv_new_network_object, this, &WidgetNetwrokObjectView::slot_recv_new_network_object);
    connect(AppSignal::getInstance(), &AppSignal::sgl_update_network_object, this, &WidgetNetwrokObjectView::slot_update_network_object);
    connect(AppSignal::getInstance(), &AppSignal::sgl_delete_network_object, this, &WidgetNetwrokObjectView::slot_delete_network_object);
}

void WidgetNetwrokObjectView::slot_view_custom_context_menu_requested(const QPoint &pos)
{
    // 渲染管道右键菜单
    QModelIndex index = ui->tvNetworkObject->indexAt(pos);
    if (!index.isValid()) return;

    QStandardItem *item = mModelSockets->itemFromIndex(index);
    if (nullptr == item) return;
    QString token = item->data(Qt::UserRole + 1).toString();
    int16_t dwConnID = item->data(Qt::UserRole + 6).toInt();
    NetworkObject *netObj = mMapNetworkObject.value(token);
    if (nullptr == netObj) return;

    int type = netObj->getObjectDetail(dwConnID).type;

    if (type <= Udp_Server)
    {
        QMenu menu(this);
        menu.setWindowFlags(menu.windowFlags() | Qt::NoDropShadowWindowHint | Qt::FramelessWindowHint);
        menu.setAttribute(Qt::WA_TranslucentBackground);
        int status = item->data(Qt::UserRole + 2).toUInt();
        QAction actionStart("开始监听");
        actionStart.setEnabled(status >= Server_Stoping);
        connect(&actionStart, &QAction::triggered, [token]()
        {
            emit AppSignal::getInstance()->sgl_start_network_object(token);
        });
        QAction actionStop("停止监听");
        actionStop.setDisabled(status >= Server_Stoping);
        connect(&actionStop, &QAction::triggered, [token]()
        {
            emit AppSignal::getInstance()->sgl_stop_network_object(token);
        });
        QAction actiondelete_server("删除服务");
        connect(&actiondelete_server, &QAction::triggered, [token]()
        {
            emit AppSignal::getInstance()->sgl_delete_network_object(token);
        });
        QAction actionDeleteConnect("删除所有连接");
        actionDeleteConnect.setEnabled(status == Server_Started);
        connect(&actionDeleteConnect, &QAction::triggered, [token]()
        {
            emit AppSignal::getInstance()->sgl_clear_network_object(token);
        });

        menu.addAction(&actionStart);
        menu.addAction(&actionStop);
        menu.addAction(&actiondelete_server);
        menu.addAction(&actionDeleteConnect);

        menu.exec(QCursor::pos());
    }
    else if (type <= Udp_Client_Slave)
    {
        QMenu menu(this);
        menu.setWindowFlags(menu.windowFlags() | Qt::NoDropShadowWindowHint | Qt::FramelessWindowHint);
        menu.setAttribute(Qt::WA_TranslucentBackground);
        QAction actionDeleteClient("删除连接");
        connect(&actionDeleteClient, &QAction::triggered, [netObj, dwConnID]()
        {
            emit AppSignal::getInstance()->sgl_stop_network_object(netObj->getObjectDetail().token, dwConnID);
        });

        menu.addAction(&actionDeleteClient);
        menu.exec(QCursor::pos());
    }
    else if (type <= Udp_Client_Cast)
    {
        QMenu menu(this);
        menu.setWindowFlags(menu.windowFlags() | Qt::NoDropShadowWindowHint | Qt::FramelessWindowHint);
        menu.setAttribute(Qt::WA_TranslucentBackground);
        int status = item->data(Qt::UserRole + 2).toUInt();

        QAction actionReconnect("重新连接");
        actionReconnect.setEnabled(status >= Client_Stoping);
        connect(&actionReconnect, &QAction::triggered, [token]()
        {
            emit AppSignal::getInstance()->sgl_start_network_object(token);
        });
        menu.addAction(&actionReconnect);

        QAction actionDisconnect("关闭连接");
        actionDisconnect.setDisabled(status >= Client_Stoping);
        connect(&actionDisconnect, &QAction::triggered, [token]()
        {
            emit AppSignal::getInstance()->sgl_stop_network_object(token);
        });
        menu.addAction(&actionDisconnect);

        QAction actionDeleteClient("删除连接");
        connect(&actionDeleteClient, &QAction::triggered, this, [token, item, this]()
        {
            mMapNetworkObject.remove(token);
            mModelSockets->removeRow(item->row());
            emit AppSignal::getInstance()->sgl_delete_network_object(token);
        });
        menu.addAction(&actionDeleteClient);

        menu.exec(QCursor::pos());
    }
}

void WidgetNetwrokObjectView::slot_create_network_object_finish(NetworkObject *netObj)
{
    if (nullptr == netObj) return;
    NetworkObjectDetail detail = netObj->getObjectDetail();

    QIcon icon = (detail.type > 1) ? QIcon(":/Resource/image/public/client.png") : QIcon(":/Resource/image/public/server.png");
    QStandardItem *item = new QStandardItem(icon, detail.name);
    item->setData(detail.token, Qt::UserRole + 1);
    item->setData(0, Qt::UserRole + 3);

    item->setData(-1, Qt::UserRole + 6);
    mModelSockets->appendRow(item);

    mMapNetworkObject.insert(detail.token, netObj);
}

void WidgetNetwrokObjectView::slot_recv_new_network_object(NetworkObject *netObj, uint16_t dwConnID)
{
    qDebug() << "WidgetNetwrokObjectView::slot_recv_new_network_object " << (nullptr == netObj);
    if (nullptr == netObj) return;
    NetworkObjectDetail detail = netObj->getObjectDetail();
    QString serverToken = detail.token;

    QModelIndexList list = mModelSockets->match(mModelSockets->index(0, 0), Qt::UserRole + 1, serverToken, 1, Qt::MatchRecursive | Qt::MatchExactly);
    qDebug() << "slot_recv_new_network_object size " << list.size();
    if (list.isEmpty()) return;

    detail = netObj->getObjectDetail(dwConnID);

    QIcon icon = QIcon(":/Resource/image/public/client.png");
    QStandardItem *item = new QStandardItem(icon, detail.name);
    item->setData(detail.token, Qt::UserRole + 1);
    item->setData(Client_Connected, Qt::UserRole + 2);
    item->setData(1, Qt::UserRole + 3); // 用于控制子项目的左间距
    item->setData(dwConnID, Qt::UserRole + 6);

    QStandardItem *parentNode = mModelSockets->itemFromIndex(list.at(0));
    parentNode->appendRow(item);

    mMapNetworkObject.insert(detail.token, netObj);

    // 更新右侧界面内容
    emit AppSignal::getInstance()->sgl_update_network_object(serverToken);
}

void WidgetNetwrokObjectView::slot_update_network_object(const QString &token)
{
    qDebug() << "WidgetNetwrokObjectView::slot_update_network_object";
    QModelIndexList list = mModelSockets->match(mModelSockets->index(0, 0), Qt::UserRole + 1, token, 1, Qt::MatchRecursive | Qt::MatchExactly);
    if (list.isEmpty()) return;

    QStandardItem *item = mModelSockets->itemFromIndex(list.at(0));
    NetworkObject *netObj = mMapNetworkObject.value(token);
    if (nullptr == netObj) return;
    qDebug() << "status  " << netObj->getObjectDetail().status;
    item->setData(netObj->getObjectDetail().status, Qt::UserRole + 2);
}

void WidgetNetwrokObjectView::slot_delete_network_object(const QString &token)
{
    QModelIndexList list = mModelSockets->match(mModelSockets->index(0, 0), Qt::UserRole + 1, token, 1, Qt::MatchRecursive | Qt::MatchExactly);
    if (list.isEmpty()) return;
    mModelSockets->removeRow(list.at(0).row(), list.at(0).parent());
}

void WidgetNetwrokObjectView::slot_current_network_object_change(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous);
    if (!current.isValid()) return;
    QStandardItem *item = mModelSockets->itemFromIndex(current);
    if (nullptr == item) return;
    QString token = item->data(Qt::UserRole + 1).toString();
    qDebug() << "token " << token;
    emit AppSignal::getInstance()->sgl_current_network_object_change(token);
}

void WidgetNetwrokObjectView::slot_network_object_click(const QModelIndex &index)
{
    if (!index.isValid()) return;
    slot_current_network_object_change(index,index);

    qDebug() << "slot_network_object_click";
}
