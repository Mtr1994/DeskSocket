#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QByteArray>
#include "HPSocket/HPSocket.h"
#include "Public/appsignal.h"
#include "Public/defines.h"

// test
#include <QDebug>
#include <QThread>

namespace mtr {

class TcpSocketListener : public QObject, public CTcpServerListener
{
    Q_OBJECT
public:
    explicit TcpSocketListener(QObject *parent = nullptr) : QObject(parent)
    {

    }

    EnHandleResult OnPrepareListen(ITcpServer* pSender, SOCKET soListen)
    {
        Q_UNUSED(pSender);Q_UNUSED(soListen);
        if (nullptr == pSender) return HR_OK;

        QString ipv4 = getServerAddress(pSender);
        USHORT usPort = getServerPort(pSender);

        emit AppSignal::getInstance()->sgl_tcp_server_status_change(ipv4, usPort, Tcp_Server_On);

        // 由于在关闭服务的时候无法获取监听地址，需要在此处记录
        mServerAddress = ipv4;
        mServerPort = usPort;
        return HR_OK;
    }

    EnHandleResult OnAccept(ITcpServer* pSender, CONNID dwConnID, UINT_PTR soClient)
    {
        Q_UNUSED(soClient);
        QString ip = getServerAddress(pSender);
        USHORT port = getServerPort(pSender);

        QString key = QString("%1:%2").arg(ip, QString::number(port));

        ip = getSocketAddress(pSender, dwConnID);
        port = getSocketPort(pSender, dwConnID);

        emit AppSignal::getInstance()->sgl_recv_new_slave_tcp_client(key, ip, port, dwConnID);
        return HR_OK;
    }

    EnHandleResult OnReceive(ITcpServer* pSender, CONNID dwConnID, const BYTE* pData, int iLength)
    {
        QString ip = getServerAddress(pSender);
        USHORT port = getServerPort(pSender);

        QString key = QString("%1:%2").arg(ip, QString::number(port));

        ip = getSocketAddress(pSender, dwConnID);
        port = getSocketPort(pSender, dwConnID);

        QByteArray data = QByteArray::fromRawData((const char*)pData, iLength);
        emit AppSignal::getInstance()->sgl_thread_recv_slave_tcp_client_data(key, ip, port, dwConnID, data);
        return HR_OK;
    }

    EnHandleResult OnReceive(ITcpServer* pSender, CONNID dwConnID, int iLength)
    {
        Q_UNUSED(pSender);Q_UNUSED(dwConnID);Q_UNUSED(iLength);
        return HR_OK;
    }

    EnHandleResult OnClose(ITcpServer* pSender, CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode)
    {
        Q_UNUSED(pSender);Q_UNUSED(dwConnID);Q_UNUSED(enOperation);Q_UNUSED(iErrorCode);
        QString socketIP = getSocketAddress(pSender, dwConnID);
        USHORT socketPort = getSocketPort(pSender, dwConnID);

        QString socketkey = QString("%1:%2*%3*%4*%5").arg(mServerAddress, QString::number(mServerPort), socketIP, QString::number(socketPort), QString::number(dwConnID));
        emit AppSignal::getInstance()->sgl_close_slave_tcp_client_result(socketkey);

        return HR_OK;
    }

    EnHandleResult OnShutdown(ITcpServer* pSender)
    {
        Q_UNUSED(pSender);
        emit AppSignal::getInstance()->sgl_tcp_server_status_change(mServerAddress, mServerPort, Tcp_Server_Shutdown);
        return HR_OK;
    }

private:
    QString getServerAddress(ITcpServer* pSender)
    {
        TCHAR szAddress[100];
        int iAddressLen = sizeof(szAddress) / sizeof(TCHAR);
        USHORT usPort = 0;
        pSender->GetListenAddress(szAddress, iAddressLen, usPort);
        return QString::fromStdWString(szAddress);
    }

    uint16_t getServerPort(ITcpServer* pSender)
    {
        TCHAR szAddress[100];
        int iAddressLen = sizeof(szAddress) / sizeof(TCHAR);
        USHORT usPort = 0;
        pSender->GetListenAddress(szAddress, iAddressLen, usPort);
        return usPort;
    }
    QString getSocketAddress(ITcpServer* pSender, CONNID dwConnID)
    {
        TCHAR szAddress[100];
        int iAddressLen = sizeof(szAddress) / sizeof(TCHAR);
        USHORT usPort = 0;
        pSender->GetRemoteAddress(dwConnID, szAddress, iAddressLen, usPort);
        return QString::fromStdWString(szAddress);
    }

    uint16_t getSocketPort(ITcpServer* pSender, CONNID dwConnID)
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

class TcpServer : public QObject
{
    Q_OBJECT
public:
    explicit TcpServer(const QString &ip, uint32_t port, QObject *parent = nullptr);
    ~TcpServer();

    bool start();

    void stop();

    bool restart();

    void closeAllConnection();

    QString getServerKey();

    bool write(CONNID dwConnID, const QByteArray &data);

    bool closeSocket(CONNID dwConnID);

    QString getSocketAddress( CONNID dwConnID);

    uint16_t getSocketPort(CONNID dwConnID);

private:
    QString mIPV4;
    uint32_t mPort;

    // Server
    CTcpServerPtr mTcpPackServer;

    // Listener
    TcpSocketListener mTcpSocketListener;
};

}

#endif // TCPSERVER_H
