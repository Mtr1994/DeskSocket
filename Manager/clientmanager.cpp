#include "clientmanager.h"
#include "Net/tcpsocket.h"
#include "Net/udpsocket.h"
#include "Public/appsignal.h"

#include <QHostAddress>

using namespace mtr;

ClientManager::ClientManager(QObject *parent) : QObject(parent)
{

}

ClientManager *ClientManager::getInstance()
{
    static ClientManager manager;
    return &manager;
}

void ClientManager::init()
{
    // 信号管理
    connect(AppSignal::getInstance(), &AppSignal::sgl_add_new_client, this, &ClientManager::slot_add_new_client);
    connect(AppSignal::getInstance(), &AppSignal::sgl_client_sent_data, this, &ClientManager::slot_client_sent_data);
    connect(AppSignal::getInstance(), &AppSignal::sgl_delete_client, this, &ClientManager::slot_delete_client);

    connect(AppSignal::getInstance(), &AppSignal::sgl_reconnect_client, this, &ClientManager::slot_connect_client);
    connect(AppSignal::getInstance(), &AppSignal::sgl_disconnect_client, this, &ClientManager::slot_disconnect_client);
}

void ClientManager::slot_add_new_client(uint16_t protocol, const QString &ipv4, uint16_t port)
{
    BaseSocket *socket = nullptr;
    if (protocol == TCP)
    {
        socket = new TcpSocket;
        socket->connect(ipv4, port);
    }
    else if (protocol == UDP)
    {
        socket = new UdpSocket;
        socket->connect(ipv4, port);
    }
    else
    {
        // 异常日志输出
    }
    mListSocket.append(socket);
}

void ClientManager::slot_client_sent_data(const QString &serverkey, const QString &contentKey, uint64_t dwconnid, const QByteArray &data)
{
    Q_UNUSED(dwconnid);
    bool status = false;
    QString error = "未找到正确的服务";
    for (auto &socket : mListSocket)
    {
        QString key = socket->getServerKey();
        if (key == serverkey)
        {
            status = socket->write(data);
            if (status) error = "";
            break;
        }
    }
}

void ClientManager::slot_delete_client(const QString &serverkey)
{
    bool status = false;
    for (auto &socket : mListSocket)
    {
        QString key = socket->getServerKey();
        if (key == serverkey)
        {
            socket->closeSocket();
            mListSocket.removeOne(socket);
            delete socket;
            status = true;
            break;
        }
    }

    emit AppSignal::getInstance()->sgl_delete_client_finish(serverkey, status);
}

void ClientManager::slot_disconnect_client(const QString &serverkey)
{
    for (auto &socket : mListSocket)
    {
        QString key = socket->getServerKey();
        if (key == serverkey)
        {
            socket->closeSocket();
            break;
        }
    }
}

void ClientManager::slot_connect_client(const QString &serverkey)
{
    for (auto &socket : mListSocket)
    {
        QString key = socket->getServerKey();
        if (key == serverkey)
        {
            socket->connect();
            break;
        }
    }
}
