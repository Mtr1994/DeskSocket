#ifndef UDPSOCKET_H
#define UDPSOCKET_H
#include "HPSocket/HPSocket.h"
#include "Public/appsignal.h"
#include "Public/defines.h"
#include "basesocket.h"

#include <QObject>

namespace mtr {

class UdpClientListener : public QObject, public CUdpClientListener
{
    Q_OBJECT
public:
    explicit UdpClientListener(QObject *parent = nullptr) : QObject(parent) {};

    EnHandleResult OnSend(IUdpClient* pSender, CONNID dwConnID, const BYTE* pData, int iLength)
    {
        // 数据发送成功的回调
        QString serverIP = getServerAddress(pSender);
        USHORT serverPort = getServerPort(pSender);
        QString socketIP = getSocketAddress(pSender);
        USHORT socketPort = getSocketPort(pSender);

        QString socketkey = QString("UDPCLIENT:%1:%2*%3*%4*%5").arg(serverIP, QString::number(serverPort), socketIP, QString::number(socketPort), QString::number(dwConnID));

        QByteArray data = QByteArray::fromRawData((const char*)pData, iLength);
        emit AppSignal::getInstance()->sgl_client_sent_data_result(socketkey, true, data.length(), "数据发送完成");
        return HR_OK;
    }
    EnHandleResult OnReceive(IUdpClient* pSender, CONNID dwConnID, const BYTE* pData, int iLength)
    {
        QString ip = getServerAddress(pSender);
        USHORT port = getServerPort(pSender);
        QString key = QString("UDPCLIENT:%1:%2").arg(ip, QString::number(port));

        ip = getSocketAddress(pSender);
        port = getSocketPort(pSender);

        QByteArray data = QByteArray::fromRawData((const char*)pData, iLength);
        emit AppSignal::getInstance()->sgl_thread_recv_client_data(key, ip, port, dwConnID, data);
        return HR_OK;
    }
    EnHandleResult OnClose(IUdpClient* pSender, CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode)
    {
        QString serverIP = getServerAddress(pSender);
        USHORT serverPort = getServerPort(pSender);
        QString socketIP = getSocketAddress(pSender);
        USHORT socketPort = getSocketPort(pSender);

        QString socketkey = QString("UDPCLIENT:%1:%2*%3*%4*%5").arg(serverIP, QString::number(serverPort), socketIP, QString::number(socketPort), QString::number(dwConnID));
        emit AppSignal::getInstance()->sgl_client_closed(socketkey);
        return HR_OK;
    }
    EnHandleResult OnConnect(IUdpClient* pSender, CONNID dwConnID)
    {
        QString ip = getServerAddress(pSender);
        USHORT port = getServerPort(pSender);
        QString key = QString("UDPCLIENT:%1:%2").arg(ip, QString::number(port));

        ip = getSocketAddress(pSender);
        port = getSocketPort(pSender);

        emit AppSignal::getInstance()->sgl_client_connected(UDP, key, ip, port, dwConnID);
        return HR_OK;
    }
    EnHandleResult OnHandShake(IUdpClient* pSender, CONNID dwConnID)
    {
        return HR_OK;
    }

private:
    QString getServerAddress(IUdpClient* pSender)
    {
        TCHAR szAddress[100];
        int iAddressLen = sizeof(szAddress) / sizeof(TCHAR);
        USHORT usPort = 0;
        pSender->GetRemoteHost(szAddress, iAddressLen, usPort);
        return QString::fromStdWString(szAddress);
    }

    uint16_t getServerPort(IUdpClient* pSender)
    {
        TCHAR szAddress[100];
        int iAddressLen = sizeof(szAddress) / sizeof(TCHAR);
        USHORT usPort = 0;
        pSender->GetRemoteHost(szAddress, iAddressLen, usPort);
        return usPort;
    }
    QString getSocketAddress(IUdpClient* pSender)
    {
        TCHAR szAddress[100];
        int iAddressLen = sizeof(szAddress) / sizeof(TCHAR);
        USHORT usPort = 0;
        pSender->GetLocalAddress(szAddress, iAddressLen, usPort);
        return QString::fromStdWString(szAddress);
    }

    uint16_t getSocketPort(IUdpClient* pSender)
    {
        TCHAR szAddress[100];
        int iAddressLen = sizeof(szAddress) / sizeof(TCHAR);
        USHORT usPort = 0;
        pSender->GetLocalAddress(szAddress, iAddressLen, usPort);
        return usPort;
    }
};

class UdpSocket : public BaseSocket
{
    Q_OBJECT
public:
    explicit UdpSocket();

    bool connect(const QString &ipv4, uint16_t port);
    bool connect();
    QString getServerKey();
    bool write(const QByteArray &data);
    bool closeSocket();

private:
    QString mServerAddress;
    uint16_t mServerPort;

    UdpClientListener mUdpClientListener;
    CUdpClientPtr mUdpClientPtr;
};

}

#endif // UDPSOCKET_H
