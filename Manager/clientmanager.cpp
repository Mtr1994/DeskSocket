#include "clientmanager.h"
#include "Net/tcpsocket.h"
#include "Public/softconstants.h"
#include "Public/appsignal.h"

#include <QHostAddress>

//test
#include <QDebug>
#include <chrono>

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
    connect(AppSignal::getInstance(), &AppSignal::sgl_add_new_tcp_client, this, &ClientManager::slot_add_new_tcp_client);
    connect(AppSignal::getInstance(), &AppSignal::sgl_tcp_client_sent_data, this, &ClientManager::slot_tcp_client_sent_data);
    connect(AppSignal::getInstance(), &AppSignal::sgl_delete_tcp_client, this, &ClientManager::slot_delete_tcp_client);

    connect(AppSignal::getInstance(), &AppSignal::sgl_connect_tcp_client, this, &ClientManager::slot_connect_tcp_client);
    connect(AppSignal::getInstance(), &AppSignal::sgl_disconnect_tcp_client, this, &ClientManager::slot_disconnect_tcp_client);
}

void ClientManager::slot_add_new_tcp_client(const QString &ip_4, uint16_t port)
{
    // 添加 TCP 连接
    TcpSocket *scoket = new TcpSocket;
    scoket->connect(ip_4, port);
    mListTcpSocket.append(scoket);
}

void ClientManager::slot_tcp_client_sent_data(const QString &serverkey, const QString &contentKey, uint64_t dwconnid, const QByteArray &data)
{
    Q_UNUSED(dwconnid);
    bool status = false;
    QString error = "未找到正确的服务";
    for (auto &socket : mListTcpSocket)
    {
        QString key = socket->getServerKey();
        if (key == serverkey)
        {
            status = socket->write(data);
            if (status) error = "";
            break;
        }
    }

    emit AppSignal::getInstance()->sgl_tcp_client_sent_data_result(contentKey, status, data.length(), error);
}

void ClientManager::slot_delete_tcp_client(const QString &serverkey)
{
    bool status = false;
    for (auto &socket : mListTcpSocket)
    {
        QString key = socket->getServerKey();
        qDebug() << "key " << key;
        if (key == serverkey)
        {
            socket->closeSocket();
            mListTcpSocket.removeOne(socket);
            delete socket;
            status = true;
            break;
        }
    }

    emit AppSignal::getInstance()->sgl_delete_tcp_client_finish(serverkey, status);
}

void ClientManager::slot_disconnect_tcp_client(const QString &serverkey)
{
    for (auto &socket : mListTcpSocket)
    {
        QString key = socket->getServerKey();
        if (key == serverkey)
        {
            socket->closeSocket();
            break;
        }
    }
}

void ClientManager::slot_connect_tcp_client(const QString &serverkey)
{
    for (auto &socket : mListTcpSocket)
    {
        QString key = socket->getServerKey();
        if (key == serverkey)
        {
            socket->connect();
            break;
        }
    }
}
