#ifndef TCPSOCKET_H
#define TCPSOCKET_H
#include "HPSocket/HPSocket.h"
#include "Public/appsignal.h"

#include <QObject>
#include <QTcpSocket>

// test
#include <QDebug>

namespace mtr {

class TcpClientListener : public QObject, public ITcpClientListener
{
    Q_OBJECT
public:
    explicit TcpClientListener(QObject *parent = nullptr) : QObject(parent) {};

    EnHandleResult OnPrepareConnect(ITcpClient* pSender, CONNID dwConnID, SOCKET socket)
    {
        return HR_OK;
    }

    EnHandleResult OnConnect(ITcpClient* pSender, CONNID dwConnID)
    {
        QString ip = getServerAddress(pSender);
        USHORT port = getServerPort(pSender);
        QString key = QString("%1:%2").arg(ip, QString::number(port));

        ip = getSocketAddress(pSender);
        port = getSocketPort(pSender);

        emit AppSignal::getInstance()->sgl_tcp_client_connected(key, ip, port, dwConnID);
        return HR_OK;
    }

    EnHandleResult OnHandShake(ITcpClient* pSender, CONNID dwConnID)
    {
        return HR_OK;
    }

    EnHandleResult OnReceive(ITcpClient* pSender, CONNID dwConnID, const BYTE* pData, int iLength)
    {
        QString ip = getServerAddress(pSender);
        USHORT port = getServerPort(pSender);
        QString key = QString("%1:%2").arg(ip, QString::number(port));

        ip = getSocketAddress(pSender);
        port = getSocketPort(pSender);

        QByteArray data = QByteArray::fromRawData((const char*)pData, iLength);
        emit AppSignal::getInstance()->sgl_thread_recv_tcp_client_data(key, ip, port, dwConnID, data);
        return HR_OK;
    }

    EnHandleResult OnReceive(ITcpClient* pSender, CONNID dwConnID, int iLength)
    {
        return HR_OK;
    }

    EnHandleResult OnSend(ITcpClient* pSender, CONNID dwConnID, const BYTE* pData, int iLength)
    {
        return HR_OK;
    }

    EnHandleResult OnClose(ITcpClient* pSender, CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode)
    {
        QString serverIP = getServerAddress(pSender);
        USHORT serverPort = getServerPort(pSender);
        QString socketIP = getSocketAddress(pSender);
        USHORT socketPort = getSocketPort(pSender);

        QString socketkey = QString("%1:%2*%3*%4*%5").arg(serverIP, QString::number(serverPort), socketIP, QString::number(socketPort), QString::number(dwConnID));
        qDebug() << "socketkey 1" << socketkey;
        emit AppSignal::getInstance()->sgl_tcp_client_closed(socketkey);
        return HR_OK;
    }

private:
    QString getServerAddress(ITcpClient* pSender)
    {
        TCHAR szAddress[100];
        int iAddressLen = sizeof(szAddress) / sizeof(TCHAR);
        USHORT usPort = 0;
        pSender->GetRemoteHost(szAddress, iAddressLen, usPort);
        return QString::fromStdWString(szAddress);
    }

    uint16_t getServerPort(ITcpClient* pSender)
    {
        TCHAR szAddress[100];
        int iAddressLen = sizeof(szAddress) / sizeof(TCHAR);
        USHORT usPort = 0;
        pSender->GetRemoteHost(szAddress, iAddressLen, usPort);
        return usPort;
    }
    QString getSocketAddress(ITcpClient* pSender)
    {
        TCHAR szAddress[100];
        int iAddressLen = sizeof(szAddress) / sizeof(TCHAR);
        USHORT usPort = 0;
        pSender->GetLocalAddress(szAddress, iAddressLen, usPort);
        return QString::fromStdWString(szAddress);
    }

    uint16_t getSocketPort(ITcpClient* pSender)
    {
        TCHAR szAddress[100];
        int iAddressLen = sizeof(szAddress) / sizeof(TCHAR);
        USHORT usPort = 0;
        pSender->GetLocalAddress(szAddress, iAddressLen, usPort);
        return usPort;
    }
};

class TcpSocket : public QObject
{
    Q_OBJECT
public:
    explicit TcpSocket(QObject *parent = nullptr);

    bool connect(const QString &ipv4, uint16_t port);
    bool connect();

    QString getServerKey();

    bool write(const QByteArray &data);

    bool closeSocket();

private:
    QString mServerAddress;
    uint16_t mServerPort;

    TcpClientListener mTcpClientListener;
    CTcpClientPtr mTcpClientPtr;
};

}

#endif // TCPSOCKET_H
