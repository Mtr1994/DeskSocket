#ifndef UDPSERVER_H
#define UDPSERVER_H

#include "networkobject.h"
#include "HPSocket/HPSocket.h"
#include "Public/appsignal.h"
#include "Network/udpsocket.h"

#include <QTimer>
#include <thread>

class UdpServerListener : public QObject, public CUdpServerListener
{
    Q_OBJECT
public:
    explicit UdpServerListener(NetworkObject* netObj) : mNetworkObject(netObj)
    {

    }

    inline void initNetworkListener()
    {
        mBaseToken = mNetworkObject->getObjectDetail().token;
    };

    inline void setActiveStatus(bool status) { mActiveStatus = status; };

    EnHandleResult OnPrepareListen(IUdpServer* pSender, SOCKET soListen) override
    {
        Q_UNUSED(pSender);Q_UNUSED(soListen);
        QTimer::singleShot(100, this, [this]{emit AppSignal::getInstance()->sgl_update_network_object(mBaseToken);});
        return HR_OK;
    }

    // 函数会在连接第一次发送数据的时候触发
    EnHandleResult OnAccept(IUdpServer* pSender, CONNID dwConnID, UINT_PTR pSockAddr) override
    {
        Q_UNUSED(pSockAddr);Q_UNUSED(pSender);
        emit AppSignal::getInstance()->sgl_recv_new_network_object(mNetworkObject, dwConnID);
        return HR_OK;
    }

    EnHandleResult OnHandShake(IUdpServer* pSender, CONNID dwConnID) override
    {
        Q_UNUSED(pSender); Q_UNUSED(dwConnID);
        return HR_OK;
    }

    EnHandleResult OnSend(IUdpServer* pSender, CONNID dwConnID, const BYTE* pData, int iLength) override
    {
        Q_UNUSED(pSender); Q_UNUSED(dwConnID);Q_UNUSED(pData); Q_UNUSED(iLength);
        return HR_OK;
    }

    EnHandleResult OnReceive(IUdpServer* pSender, CONNID dwConnID, int iLength) override
    {
        Q_UNUSED(pSender); Q_UNUSED(dwConnID); Q_UNUSED(iLength);
        return HR_OK;
    }

    EnHandleResult OnReceive(IUdpServer* pSender, CONNID dwConnID, const BYTE* pData, int iLength) override
    {
        Q_UNUSED(pSender);Q_UNUSED(dwConnID);
        std::string data = std::string((const char*)pData, iLength);
        emit AppSignal::getInstance()->sgl_recv_network_data(mBaseToken + QString::number(dwConnID), data, iLength);
        return HR_OK;
    }

    EnHandleResult OnClose(IUdpServer* pSender, CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode) override
    {
        Q_UNUSED(pSender);Q_UNUSED(dwConnID);Q_UNUSED(enOperation);Q_UNUSED(iErrorCode);
        emit AppSignal::getInstance()->sgl_delete_network_object(mBaseToken + QString::number(dwConnID));
        return HR_OK;
    }

    EnHandleResult OnShutdown(IUdpServer* pSender) override
    {
        Q_UNUSED(pSender);
        emit AppSignal::getInstance()->sgl_update_network_object(mBaseToken);
        return HR_OK;
    }

private:
    NetworkObject   *mNetworkObject = nullptr;
    bool            mActiveStatus = true;
    QString         mBaseToken;
};

class UdpServer : public NetworkObject
{
    Q_OBJECT
public:
    explicit UdpServer(const QString& address, uint16_t port, QObject *parent = nullptr);
    void start() override;
    void stop(int32_t dwConnID) override;
    void clear() override;
    void close()override;
    void send(const std::string &data, uint32_t length, int32_t dwConnID) override;
    NetworkObjectDetail getObjectDetail(int32_t dwConnID) override;

private:
    QString mLocalAddress;
    uint16_t mLocalPort;

    CUdpServerPtr mUdpServer;
    UdpServerListener mUdpServerListener;
};

#endif // UDPSERVER_H
