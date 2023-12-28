#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include "networkobject.h"
#include "HPSocket/HPSocket.h"
#include "Public/appsignal.h"

#include <QObject>

// test
#include <QDebug>

class TcpClientListener : public QObject, public CTcpClientListener
{
    Q_OBJECT
public:
    explicit TcpClientListener(NetworkObject* netObj) : mNetworkObject(netObj)
    {

    };

    inline void initNetworkListener()
    {
        mBaseToken = mNetworkObject->getObjectDetail().token;
    };

    inline void setActiveStatus(bool status) { mActiveStatus = status; };

    EnHandleResult OnPrepareConnect(ITcpClient* pSender, CONNID dwConnID, SOCKET socket) override
    {
        Q_UNUSED(pSender); Q_UNUSED(dwConnID); Q_UNUSED(socket);
        return HR_OK;
    }

    EnHandleResult OnConnect(ITcpClient* pSender, CONNID dwConnID) override
    {
        Q_UNUSED(dwConnID); Q_UNUSED(pSender);
        if (!mActiveStatus) return HR_OK;
        emit AppSignal::getInstance()->sgl_update_network_object(mBaseToken);
        return HR_OK;
    }

    EnHandleResult OnHandShake(ITcpClient* pSender, CONNID dwConnID) override
    {
        Q_UNUSED(pSender); Q_UNUSED(dwConnID);
        return HR_OK;
    }

    EnHandleResult OnReceive(ITcpClient* pSender, CONNID dwConnID, const BYTE* pData, int iLength) override
    {
        Q_UNUSED(pSender); Q_UNUSED(dwConnID);
        if (!mActiveStatus) return HR_OK;
        std::string data = std::string((const char*)pData, iLength);
        emit AppSignal::getInstance()->sgl_recv_network_data(mBaseToken, data, iLength);
        return HR_OK;
    }

    EnHandleResult OnReceive(ITcpClient* pSender, CONNID dwConnID, int iLength) override
    {
        Q_UNUSED(pSender); Q_UNUSED(dwConnID); Q_UNUSED(iLength);
        return HR_OK;
    }

    EnHandleResult OnSend(ITcpClient* pSender, CONNID dwConnID, const BYTE* pData, int iLength) override
    {
        Q_UNUSED(pSender); Q_UNUSED(dwConnID); Q_UNUSED(pData); Q_UNUSED(iLength);
        return HR_OK;
    }

    EnHandleResult OnClose(ITcpClient* pSender, CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode) override
    {
        Q_UNUSED(pSender);Q_UNUSED(enOperation); Q_UNUSED(iErrorCode);Q_UNUSED(dwConnID);
        if (!mActiveStatus) return HR_OK;
        emit AppSignal::getInstance()->sgl_update_network_object(mBaseToken);

        qDebug() << "tcp OnClose " << pSender->GetState();
        return HR_OK;
    }

private:
    NetworkObject   *mNetworkObject = nullptr;
    bool            mActiveStatus = true;
    QString         mBaseToken;
};

class TcpSocket : public NetworkObject
{
    Q_OBJECT
public:
    explicit TcpSocket(const QString& address, uint16_t port, QObject *parent = nullptr);
    void start() override;
    void stop(int32_t dwConnID) override;
    void clear() override;
    void close()override;
    void send(const std::string &data, uint32_t length, int32_t dwConnID) override;
    NetworkObjectDetail getObjectDetail(int32_t dwConnID) override;

private:
    QString mPeerAddress;
    uint16_t mPeerPort;

    TcpClientListener mTcpClientListener;
    CTcpClientPtr mTcpClientPtr;
};

#endif // TCPSOCKET_H
