#ifndef UDPBROADCAST_H
#define UDPBROADCAST_H

#include "networkobject.h"
#include "HPSocket/HPSocket.h"
#include "Public/appsignal.h"

// test
#include <QDebug>

class UdpBroadcastListener : public QObject, public CUdpCastListener
{
    Q_OBJECT
public:
    explicit UdpBroadcastListener(NetworkObject* netObj) : mNetworkObject(netObj)
    {

    };

    inline void initNetworkListener()
    {
        mBaseToken = mNetworkObject->getObjectDetail().token;
    };

    inline void setActiveStatus(bool status) { mActiveStatus = status; };

    EnHandleResult OnPrepareConnect(IUdpCast* pSender, CONNID dwConnID, SOCKET socket) override
    {
        Q_UNUSED(pSender);Q_UNUSED(dwConnID);Q_UNUSED(socket);
        qDebug() << "udp cast OnPrepareConnect";
        return HR_OK;
    }

    EnHandleResult OnConnect(IUdpCast* pSender, CONNID dwConnID) override
    {
        Q_UNUSED(dwConnID); Q_UNUSED(pSender);
        if (!mActiveStatus) return HR_OK;
        qDebug() << "udp cast OnConnect";
        emit AppSignal::getInstance()->sgl_update_network_object(mBaseToken);
        return HR_OK;
    }

    EnHandleResult OnSend(IUdpCast* pSender, CONNID dwConnID, const BYTE* pData, int iLength) override
    {
        return HR_OK;
    }

    using CUdpCastListener::OnReceive;
    EnHandleResult OnReceive(IUdpCast* pSender, CONNID dwConnID, const BYTE* pData, int iLength) override
    {
        Q_UNUSED(pSender);Q_UNUSED(dwConnID);Q_UNUSED(pData);Q_UNUSED(iLength);
        // 由于发送端口默认等于广播端口，程序会收到自己发送的广播数据包
        // 此互踩不予处理（显示）
        return HR_OK;
    }

    EnHandleResult OnClose(IUdpCast* pSender, CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode) override
    {
        Q_UNUSED(pSender);Q_UNUSED(dwConnID);Q_UNUSED(enOperation);Q_UNUSED(iErrorCode);
        emit AppSignal::getInstance()->sgl_update_network_object(mBaseToken);
        return HR_OK;
    }

private:
    NetworkObject   *mNetworkObject = nullptr;
    bool            mActiveStatus = true;
    QString         mBaseToken;
};

class UdpBroadcast : public NetworkObject
{
    Q_OBJECT
public:
    explicit UdpBroadcast(const QString& address, uint16_t port, QObject *parent = nullptr);
    void start() override;
    void stop(int32_t dwConnID) override;
    void clear() override;
    void close()override;
    void send(const std::string &data, uint32_t length, int32_t dwConnID) override;
    NetworkObjectDetail getObjectDetail(int32_t dwConnID) override;

private:
    QString mLocalAddress;
    uint16_t mLocalPort;

    UdpBroadcastListener mUdpBroadcastListener;
    CUdpCastPtr mUdpCastPtr;
};

#endif // UDPBROADCAST_H
