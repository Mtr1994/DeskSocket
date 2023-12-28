#ifndef UDPSOCKET_H
#define UDPSOCKET_H

#include "networkobject.h"
#include "HPSocket/HPSocket.h"
#include "Public/appsignal.h"

class UdpClientListener : public QObject, public CUdpClientListener
{
    Q_OBJECT
public:
    explicit UdpClientListener(NetworkObject* netObj) : mNetworkObject(netObj)
    {

    };

    inline void initNetworkListener()
    {
        mBaseToken = mNetworkObject->getObjectDetail().token;
    };

    inline void setActiveStatus(bool status) { mActiveStatus = status; };

    EnHandleResult OnConnect(IUdpClient* pSender, CONNID dwConnID) override
    {
        Q_UNUSED(pSender);Q_UNUSED(dwConnID);
        emit AppSignal::getInstance()->sgl_update_network_object(mBaseToken);
        return HR_OK;
    }

    EnHandleResult OnHandShake(IUdpClient* pSender, CONNID dwConnID) override
    {
        Q_UNUSED(pSender); Q_UNUSED(dwConnID);
        return HR_OK;
    }

    EnHandleResult OnSend(IUdpClient* pSender, CONNID dwConnID, const BYTE* pData, int iLength) override
    {
        Q_UNUSED(pSender);Q_UNUSED(dwConnID);
        //QByteArray data = QByteArray::fromRawData((const char*)pData, iLength);
        return HR_OK;
    }

    using CUdpClientListener::OnReceive;
    EnHandleResult OnReceive(IUdpClient* pSender, CONNID dwConnID, const BYTE* pData, int iLength) override
    {
        Q_UNUSED(pSender);Q_UNUSED(dwConnID);

        std::string data = std::string((const char*)pData, iLength);
        emit AppSignal::getInstance()->sgl_recv_network_data(mBaseToken, data, iLength);
        return HR_OK;
    }

    EnHandleResult OnClose(IUdpClient* pSender, CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode) override
    {
        Q_UNUSED(pSender);Q_UNUSED(enOperation); Q_UNUSED(iErrorCode);Q_UNUSED(dwConnID);
        emit AppSignal::getInstance()->sgl_update_network_object(mBaseToken);
        return HR_OK;
    }

private:
    NetworkObject   *mNetworkObject = nullptr;
    bool            mActiveStatus = true;
    QString         mBaseToken;
};

class UdpSocket : public NetworkObject
{
    Q_OBJECT
public:
    explicit UdpSocket(const QString& address, uint16_t port, QObject *parent = nullptr);
    void start() override;
    void stop(int32_t dwConnID) override;
    void clear() override;
    void close()override;
    void send(const std::string &data, uint32_t length, int32_t dwConnID) override;
    NetworkObjectDetail getObjectDetail(int32_t dwConnID) override;

private:
    QString mPeerAddress;
    uint16_t mPeerPort;

    UdpClientListener mUdpClientListener;
    CUdpClientPtr mUdpClientPtr;
};

#endif // UDPSOCKET_H
