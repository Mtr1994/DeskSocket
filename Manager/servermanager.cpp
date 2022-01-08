#include "servermanager.h"
#include "Public/appsignal.h"

#include "Net/tcpserver.h"

// test
#include <QDebug>
#include <QThread>

using namespace mtr;

ServerManager::ServerManager(QObject *parent) : QObject(parent)
{

}

void ServerManager::init()
{
    // 信号管理
    connect(AppSignal::getInstance(), &AppSignal::sgl_add_new_tcp_server, this, &ServerManager::slot_add_new_tcp_server);

    // 接受的连接发送数据
    connect(AppSignal::getInstance(), &AppSignal::sgl_slave_tcp_client_sent_data, this, &ServerManager::slot_slave_tcp_client_sent_data);

    // 关闭接受的连接
    connect(AppSignal::getInstance(), &AppSignal::sgl_delete_slave_tcp_client, this, &ServerManager::slot_delete_slave_tcp_client);

    // 关闭服务
    connect(AppSignal::getInstance(), &AppSignal::sgl_stop_tcp_server, this, &ServerManager::slot_stop_tcp_server);

    // 启动服务
    connect(AppSignal::getInstance(), &AppSignal::sgl_start_tcp_server, this, &ServerManager::slot_start_tcp_server);

    // 关闭所有连接
    connect(AppSignal::getInstance(), &AppSignal::sgl_close_all_slave_tcp_client, this, &ServerManager::slot_close_all_slave_tcp_client);

    // 删除服务
    connect(AppSignal::getInstance(), &AppSignal::sgl_delete_tcp_server, this, &ServerManager::slot_delete_tcp_server);
}

ServerManager* ServerManager::getInstance()
{
    static ServerManager server;
    return &server;
}

void ServerManager::slot_add_new_tcp_server(const QString &ip_4, uint16_t port)
{
    TcpServer *server = new TcpServer(ip_4, port);
    bool status = server->start();
    if (!status)
    {
        // 发送服务启动失败通知，启动成功的通知由 listener 发送
        emit AppSignal::getInstance()->sgl_tcp_server_status_change(ip_4, port, Tcp_Server_Shutdown);
    }
    mListTcpServer.append(server);
}

void ServerManager::slot_slave_tcp_client_sent_data(const QString &serverkey, const QString &contentKey, uint64_t dwconnid, const QByteArray &data)
{
    bool status = false;
    QString error = "未找到正确的服务";
    for (auto &server : mListTcpServer)
    {
        QString key = server->getServerKey();
        if (key == serverkey)
        {
            status = server->write(dwconnid, data);
            if (status) error = "";
            break;
        }
    }

    emit AppSignal::getInstance()->sgl_slave_tcp_client_sent_data_result(contentKey, status, data.length(), error);
}

void ServerManager::slot_delete_slave_tcp_client(const QString &serverkey, uint64_t dwconnid)
{
    for (auto &server : mListTcpServer)
    {
        QString key = server->getServerKey();
        if (key == serverkey)
        {
            server->closeSocket(dwconnid);
            break;
        }
    }
}

void ServerManager::slot_stop_tcp_server(const QString &serverkey)
{
    for (auto &server : mListTcpServer)
    {
        QString key = server->getServerKey();
        if (key == serverkey)
        {
            server->stop();
            break;
        }
    }
}

void ServerManager::slot_start_tcp_server(const QString &serverkey)
{
    for (auto &server : mListTcpServer)
    {
        QString key = server->getServerKey();
        if (key == serverkey)
        {
            bool status = server->restart();
            if (!status)
            {
                // 发送服务启动失败通知，启动成功的通知由 listener 发送
                QStringList list = key.split(':');
                if (list.length() != 2)
                {
                    qDebug() << "报告错误";
                }
                else
                {
                    emit AppSignal::getInstance()->sgl_tcp_server_status_change(list.at(0), list.at(1).toUShort(), Tcp_Server_Shutdown);
                }
            }
            break;
        }
    }
}

void ServerManager::slot_close_all_slave_tcp_client(const QString &serverkey)
{
    for (auto &server : mListTcpServer)
    {
        QString key = server->getServerKey();
        if (key == serverkey)
        {
            server->closeAllConnection();
            break;
        }
    }
}

void ServerManager::slot_delete_tcp_server(const QString &serverkey)
{
    for (auto &server : mListTcpServer)
    {
        QString key = server->getServerKey();
        if (key == serverkey)
        {
            server->stop();
            mListTcpServer.removeOne(server);
            delete server;

            emit AppSignal::getInstance()->sgl_delete_tcp_server_finish(serverkey);
            break;
        }
    }
}
