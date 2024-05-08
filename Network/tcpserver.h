#ifndef TCPSERVER_H
#define TCPSERVER_H

#include "networkobject.h"
#include "HPSocket/HPSocket.h"
#include "Public/appsignal.h"

#include <QTimer>
#include <thread>

class TcpSocketListener : public QObject, public CTcpServerListener
{
    Q_OBJECT
public:
    explicit TcpSocketListener(NetworkObject* netObj) : mNetworkObject(netObj)
    {

    }

    inline void initNetworkListener()
    {
        mBaseToken = mNetworkObject->getObjectDetail().token;
    };

    inline void setActiveStatus(bool status) { mActiveStatus = status; };

    EnHandleResult OnPrepareListen(ITcpServer* pSender, SOCKET soListen) override
    {
        Q_UNUSED(pSender); Q_UNUSED(soListen);
        QTimer::singleShot(100, this, [this]{emit AppSignal::getInstance()->sgl_update_network_object(mBaseToken);});
        return HR_OK;
    }

    EnHandleResult OnAccept(ITcpServer* pSender, CONNID dwConnID, UINT_PTR soClient) override
    {
        Q_UNUSED(soClient);Q_UNUSED(pSender);
        emit AppSignal::getInstance()->sgl_recv_new_network_object(mNetworkObject, dwConnID);
        return HR_OK;
    }

    EnHandleResult OnReceive(ITcpServer* pSender, CONNID dwConnID, const BYTE* pData, int iLength) override
    {
        Q_UNUSED(pSender);Q_UNUSED(dwConnID);
        std::string data = std::string((const char*)pData, iLength);
        emit AppSignal::getInstance()->sgl_recv_network_data(mBaseToken + QString::number(dwConnID), data, iLength);
        return HR_OK;
    }

    EnHandleResult OnReceive(ITcpServer* pSender, CONNID dwConnID, int iLength) override
    {
        Q_UNUSED(pSender);Q_UNUSED(dwConnID);Q_UNUSED(iLength);
        return HR_OK;
    }

    EnHandleResult OnClose(ITcpServer* pSender, CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode) override
    {
        Q_UNUSED(pSender);Q_UNUSED(dwConnID);Q_UNUSED(enOperation);Q_UNUSED(iErrorCode);
        emit AppSignal::getInstance()->sgl_delete_network_object(mBaseToken + QString::number(dwConnID));
        return HR_OK;
    }

    EnHandleResult OnShutdown(ITcpServer* pSender) override
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

class TcpServer : public NetworkObject
{
    Q_OBJECT
public:
    explicit TcpServer(const QString& address, uint16_t port, QObject *parent = nullptr);
    void start() override;
    void stop(int32_t dwConnID) override;
    void clear() override;
    void close()override;
    void edit(const QString &address, uint16_t port) override;
    void send(const std::string &data, uint32_t length, int32_t dwConnID) override;
    NetworkObjectDetail getObjectDetail(int32_t dwConnID) override;

private:
    QString mLocalAddress;
    uint16_t mLocalPort;

    CTcpServerPtr mTcpPackServer;
    TcpSocketListener mTcpSocketListener;
};

#endif // TCPSERVER_H
