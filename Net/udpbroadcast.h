#ifndef UDPBROADCAST_H
#define UDPBROADCAST_H
#include "HPSocket/HPSocket.h"
#include "Public/appsignal.h"
#include "Public/defines.h"
#include "basesocket.h"

#include <QObject>

namespace mtr {

class UdpBroadcastListener : public QObject, public CUdpCastListener
{
    Q_OBJECT
public:
    explicit UdpBroadcastListener(QObject *parent = nullptr) : QObject(parent) {};

    EnHandleResult OnPrepareConnect(IUdpCast* pSender, CONNID dwConnID, SOCKET socket)
    {
        Q_UNUSED(pSender);Q_UNUSED(dwConnID);Q_UNUSED(socket);
        return HR_OK;
    }
    EnHandleResult OnConnect(IUdpCast* pSender, CONNID dwConnID)
    {
        QString ip = getServerAddress(pSender);
        USHORT port = getServerPort(pSender);
        QString key = QString("UDPCLIENT:%1:%2").arg(ip, QString::number(port));

        ip = getSocketAddress(pSender);
        port = getSocketPort(pSender);

        emit AppSignal::getInstance()->sgl_client_connected(UDP, key, ip, port, dwConnID);
        return HR_OK;
    }
    EnHandleResult OnSend(IUdpCast* pSender, CONNID dwConnID, const BYTE* pData, int iLength)
    {
        QString serverIP = getServerAddress(pSender);
        USHORT serverPort = getServerPort(pSender);
        QString socketIP = getSocketAddress(pSender);
        USHORT socketPort = getSocketPort(pSender);

        QString socketkey = QString("UDPCLIENT:%1:%2*%3*%4*%5").arg(serverIP, QString::number(serverPort), socketIP, QString::number(socketPort), QString::number(dwConnID));

        QByteArray data = QByteArray::fromRawData((const char*)pData, iLength);
        emit AppSignal::getInstance()->sgl_client_sent_data_result(socketkey, true, data.length(), "数据发送完成");
        return HR_OK;
    }
    EnHandleResult OnReceive(IUdpCast* pSender, CONNID dwConnID, const BYTE* pData, int iLength)
    {
        Q_UNUSED(pSender);Q_UNUSED(dwConnID);Q_UNUSED(pData);Q_UNUSED(iLength);
        // 由于发送端口默认等于广播端口，程序会收到自己发送的广播数据包
        // 此互踩不予处理（显示）
        return HR_OK;
    }
    EnHandleResult OnClose(IUdpCast* pSender, CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode)
    {
        Q_UNUSED(enOperation);Q_UNUSED(iErrorCode);
        QString serverIP = getServerAddress(pSender);
        USHORT serverPort = getServerPort(pSender);
        QString socketIP = getSocketAddress(pSender);
        USHORT socketPort = getSocketPort(pSender);

        QString socketkey = QString("UDPCLIENT:%1:%2*%3*%4*%5").arg(serverIP, QString::number(serverPort), socketIP, QString::number(socketPort), QString::number(dwConnID));
        emit AppSignal::getInstance()->sgl_client_closed(socketkey);
        return HR_OK;
    }

private:
    QString getServerAddress(IUdpCast* pSender)
    {
        TCHAR szAddress[100];
        int iAddressLen = sizeof(szAddress) / sizeof(TCHAR);
        USHORT usPort = 0;
        pSender->GetRemoteHost(szAddress, iAddressLen, usPort);
        return QString::fromStdWString(szAddress);
    }

    uint16_t getServerPort(IUdpCast* pSender)
    {
        TCHAR szAddress[100];
        int iAddressLen = sizeof(szAddress) / sizeof(TCHAR);
        USHORT usPort = 0;
        pSender->GetRemoteHost(szAddress, iAddressLen, usPort);
        return usPort;
    }
    QString getSocketAddress(IUdpCast* pSender)
    {
        TCHAR szAddress[100];
        int iAddressLen = sizeof(szAddress) / sizeof(TCHAR);
        USHORT usPort = 0;
        pSender->GetLocalAddress(szAddress, iAddressLen, usPort);
        return QString::fromStdWString(szAddress);
    }
    uint16_t getSocketPort(IUdpCast* pSender)
    {
        TCHAR szAddress[100];
        int iAddressLen = sizeof(szAddress) / sizeof(TCHAR);
        USHORT usPort = 0;
        pSender->GetLocalAddress(szAddress, iAddressLen, usPort);
        return usPort;
    }

private:
    QString mServerAddress;
    uint16_t mServerPort;
};

class UdpBroadcast : public BaseSocket
{
    Q_OBJECT
public:
    explicit UdpBroadcast();

    bool connect(const QString &ipv4, uint16_t port);
    bool connect();
    QString getServerKey();
    bool write(const QByteArray &data);
    bool closeSocket();

private:
    QString mServerAddress;
    uint16_t mServerPort;

    UdpBroadcastListener mUdpBroadcastListener;
    CUdpCastPtr mUdpCastPtr;
};

}

#endif // UDPBROADCAST_H
