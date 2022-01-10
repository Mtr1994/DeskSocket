#include "servermanager.h"
#include "Public/appsignal.h"

#include "Net/tcpserver.h"
#include "Net/udpserver.h"
#include "Log/logger.h"

using namespace mtr;

ServerManager::ServerManager(QObject *parent) : QObject(parent)
{

}

void ServerManager::init()
{
    // 信号管理
    connect(AppSignal::getInstance(), &AppSignal::sgl_add_new_server, this, &ServerManager::slot_add_new_server);

    // 接受的连接发送数据
    connect(AppSignal::getInstance(), &AppSignal::sgl_slave_client_sent_data, this, &ServerManager::slot_slave_client_sent_data);

    // 关闭接受的连接
    connect(AppSignal::getInstance(), &AppSignal::sgl_delete_slave_client, this, &ServerManager::slot_delete_slave_client);

    // 关闭服务
    connect(AppSignal::getInstance(), &AppSignal::sgl_stop_server, this, &ServerManager::slot_stop_server);

    // 启动服务
    connect(AppSignal::getInstance(), &AppSignal::sgl_restart_server, this, &ServerManager::slot_restart_server);

    // 关闭所有连接
    connect(AppSignal::getInstance(), &AppSignal::sgl_close_all_slave_client, this, &ServerManager::slot_close_all_slaveclient);

    // 删除服务
    connect(AppSignal::getInstance(), &AppSignal::sgl_delete_server, this, &ServerManager::slot_delete_server);

}

ServerManager* ServerManager::getInstance()
{
    static ServerManager server;
    return &server;
}

void ServerManager::slot_add_new_server(uint16_t protocol, const QString &ipv4, uint16_t port)
{
    BaseServer *server = nullptr;
    if (protocol == TCP) server = new TcpServer(ipv4, port);
    else if (protocol == UDP) server = new UdpServer(ipv4, port);
    else LOG_INFO("unknow protocol");

    bool status = server->start();
    if (!status)
    {
        // 发送服务启动失败通知，启动成功的通知由 listener 发送
        emit AppSignal::getInstance()->sgl_server_status_change(protocol, ipv4, port, Server_Shutdown);
    }
    mListServer.append(server);
}

void ServerManager::slot_slave_client_sent_data(const QString &serverkey, const QString &contentKey, uint64_t dwconnid, const QByteArray &data)
{
    bool status = false;
    QString error = "未找到正确的服务";
    for (auto &server : mListServer)
    {
        QString key = server->getServerKey();
        if (key == serverkey)
        {
            status = server->write(dwconnid, data);
            if (status) error = "";
            break;
        }
    }

    emit AppSignal::getInstance()->sgl_slave_client_sent_data_result(contentKey, status, data.length(), error);
}

void ServerManager::slot_delete_slave_client(const QString &serverkey, uint64_t dwconnid)
{
    for (auto &server : mListServer)
    {
        QString key = server->getServerKey();
        if (key == serverkey)
        {
            server->closeSocket(dwconnid);
            break;
        }
    }
}

void ServerManager::slot_stop_server(const QString &serverkey)
{
    for (auto &server : mListServer)
    {
        QString key = server->getServerKey();
        if (key == serverkey)
        {
            server->stop();
            break;
        }
    }
}

void ServerManager::slot_restart_server(const QString &serverkey)
{
    for (auto &server : mListServer)
    {
        QString key = server->getServerKey();
        if (key == serverkey)
        {
            bool status = server->restart();
            if (!status)
            {
                // 发送服务启动失败通知，启动成功的通知由 listener 发送
                QStringList list = key.split(':');
                if (list.length() != 3)
                {
                    LOG_INFO("系统异常");
                }
                else
                {
                    uint16_t protocol = list.at(0).contains("TCP") ? TCP : UDP;
                    emit AppSignal::getInstance()->sgl_server_status_change(protocol, list.at(1), list.at(2).toUShort(), Server_Shutdown);
                }
            }
            break;
        }
    }
}

void ServerManager::slot_close_all_slaveclient(const QString &serverkey)
{
    for (auto &server : mListServer)
    {
        QString key = server->getServerKey();
        if (key == serverkey)
        {
            server->clearClient();
            break;
        }
    }
}

void ServerManager::slot_delete_server(const QString &serverkey)
{
    for (auto &server : mListServer)
    {
        QString key = server->getServerKey();
        if (key == serverkey)
        {
            server->stop();
            mListServer.removeOne(server);
            delete server;

            emit AppSignal::getInstance()->sgl_delete_server_finish(serverkey);
            break;
        }
    }
}
