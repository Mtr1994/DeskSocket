#include "networkmanager.h"
#include "Public/appsignal.h"
#include "Public/defines.h"
#include "Network/tcpsocket.h"
#include "Network/udpsocket.h"
#include "Network/tcpserver.h"
#include "Network/udpserver.h"
#include "Network/udpbroadcast.h"

#include <QThread>

// test
#include <QDebug>

NetworkManager::NetworkManager(QObject *parent)
    : QObject{parent}
{

}

NetworkManager *NetworkManager::getInstance()
{
    static NetworkManager manager;
    return &manager;
}

void NetworkManager::init()
{
    // 信号管理
    connect(AppSignal::getInstance(), &AppSignal::sgl_prepare_network_object, this, &NetworkManager::slot_prepare_network_object);
}

void NetworkManager::slot_prepare_network_object(int type, const QString &address, uint16_t port)
{
    NetworkObject *object = nullptr;
    if (type == Tcp_Server) object = new TcpServer(address, port);
    else if (type == Tcp_Client) object = new TcpSocket(address, port);
    else if (type == Udp_Server) object = new UdpServer(address, port);
    else if (type == Udp_Client) object = new UdpSocket(address, port);
    else if (type == Udp_Client_Cast) object = new UdpBroadcast(address, port);

    if (nullptr == object) return;

    connect(AppSignal::getInstance(), &AppSignal::sgl_start_network_object, object, &NetworkObject::slot_start_network_object);
    connect(AppSignal::getInstance(), &AppSignal::sgl_stop_network_object, object, &NetworkObject::slot_stop_network_object);
    connect(AppSignal::getInstance(), &AppSignal::sgl_clear_network_object, object, &NetworkObject::slot_clear_network_object);
    connect(AppSignal::getInstance(), &AppSignal::sgl_delete_network_object, object, &NetworkObject::slot_delete_network_object);

    emit AppSignal::getInstance()->sgl_create_network_object_finish(object);

    QThread *thread = new QThread;
    object->moveToThread(thread);
    connect(object, &NetworkObject::sgl_delete_network_object_finish, this, [thread]
    {
        thread->exit(0); thread->wait();
        thread->deleteLater();
    });
    connect(thread, &QThread::started, object, &NetworkObject::slot_init_network_object);
    thread->start();
}
