#ifndef UDPSERVER_H
#define UDPSERVER_H

#include <QObject>
#include "HPSocket/HPSocket.h"
#include "Public/appsignal.h"
#include "Public/defines.h"
#include "baseserver.h"

namespace mtr {

class UdpServerListener : public QObject, public CUdpServerListener
{
    Q_OBJECT
public:
    explicit UdpServerListener(QObject *parent = nullptr) : QObject(parent)
    {

    }

    EnHandleResult OnPrepareListen(IUdpServer* pSender, SOCKET soListen)
    {
        if (nullptr == pSender) return HR_OK;

        QString ipv4 = getServerAddress(pSender);
        USHORT usPort = getServerPort(pSender);

        emit AppSignal::getInstance()->sgl_server_status_change(UDP, ipv4, usPort, Server_Started);

        // 由于在关闭服务的时候无法获取监听地址，需要在此处记录
        mServerAddress = ipv4;
        mServerPort = usPort;
        return HR_OK;
    }
    // 函数会在连接第一次发送数据的时候触发
    EnHandleResult OnAccept(IUdpServer* pSender, CONNID dwConnID, UINT_PTR pSockAddr)
    {
        Q_UNUSED(pSockAddr);
        QString key = QString("UDPSERVER:%1:%2").arg(mServerAddress, QString::number(mServerPort));

        QString ip = getSocketAddress(pSender, dwConnID);
        uint16_t port = getSocketPort(pSender, dwConnID);

        emit AppSignal::getInstance()->sgl_recv_new_slave_client(UDP, key, ip, port, dwConnID);
        return HR_OK;
    }
    EnHandleResult OnHandShake(IUdpServer* pSender, CONNID dwConnID)
    {
        Q_UNUSED(pSender); Q_UNUSED(dwConnID);
        return HR_OK;
    }
    EnHandleResult OnSend(IUdpServer* pSender, CONNID dwConnID, const BYTE* pData, int iLength)
    {
        return HR_OK;
    }
    EnHandleResult OnReceive(IUdpServer* pSender, CONNID dwConnID, const BYTE* pData, int iLength)
    {
        QString key = QString("UDPSERVER:%1:%2").arg(mServerAddress, QString::number(mServerPort));

        QString ip = getSocketAddress(pSender, dwConnID);
        USHORT port = getSocketPort(pSender, dwConnID);

        QByteArray data = QByteArray::fromRawData((const char*)pData, iLength);
        emit AppSignal::getInstance()->sgl_thread_recv_slave_client_data(key, ip, port, dwConnID, data.toStdString());
        return HR_OK;
    }

    EnHandleResult OnClose(IUdpServer* pSender, CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode)
    {
        Q_UNUSED(pSender);Q_UNUSED(dwConnID);Q_UNUSED(enOperation);Q_UNUSED(iErrorCode);
        QString socketIP = getSocketAddress(pSender, dwConnID);
        USHORT socketPort = getSocketPort(pSender, dwConnID);

        QString socketkey = QString("UDPSERVER:%1:%2*%3*%4*%5").arg(mServerAddress, QString::number(mServerPort), socketIP, QString::number(socketPort), QString::number(dwConnID));
        emit AppSignal::getInstance()->sgl_close_slave_client_result(socketkey);

        return HR_OK;
    }
    EnHandleResult OnShutdown(IUdpServer* pSender)
    {
        Q_UNUSED(pSender);
        emit AppSignal::getInstance()->sgl_server_status_change(UDP, mServerAddress, mServerPort, Server_Shutdown);
        return HR_OK;
    }

private:
    QString getServerAddress(IUdpServer* pSender)
    {
        TCHAR szAddress[100];
        int iAddressLen = sizeof(szAddress) / sizeof(TCHAR);
        USHORT usPort = 0;
        pSender->GetListenAddress(szAddress, iAddressLen, usPort);
        return QString::fromStdWString(szAddress);
    }
    uint16_t getServerPort(IUdpServer* pSender)
    {
        TCHAR szAddress[100];
        int iAddressLen = sizeof(szAddress) / sizeof(TCHAR);
        USHORT usPort = 0;
        pSender->GetListenAddress(szAddress, iAddressLen, usPort);
        return usPort;
    }
    QString getSocketAddress(IUdpServer* pSender, CONNID dwConnID)
    {
        TCHAR szAddress[100];
        int iAddressLen = sizeof(szAddress) / sizeof(TCHAR);
        USHORT usPort = 0;
        pSender->GetRemoteAddress(dwConnID, szAddress, iAddressLen, usPort);
        return QString::fromStdWString(szAddress);
    }

    uint16_t getSocketPort(IUdpServer* pSender, CONNID dwConnID)
    {
        TCHAR szAddress[100];
        int iAddressLen = sizeof(szAddress) / sizeof(TCHAR);
        USHORT usPort = 0;
        pSender->GetRemoteAddress(dwConnID, szAddress, iAddressLen, usPort);
        return usPort;
    }

private:
    QString mServerAddress;
    uint16_t mServerPort;
};

class UdpServer : public BaseServer
{
    Q_OBJECT
public:
    explicit UdpServer(const QString &ip, uint16_t port);

    bool start();

    void stop();

    bool restart();

    void clearClient();

    bool write(CONNID dwConnID, const std::string &data);

    bool closeSocket(CONNID dwConnID);

    QString getServerKey();

private:
    QString mServerAddress;
    uint16_t mServerPort;

    // Server
    CUdpServerPtr mUdpServer;

    // Listener
    UdpServerListener mUdpServerListener;
};

}

#endif // UDPSERVER_H
