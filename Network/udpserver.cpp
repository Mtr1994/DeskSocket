#include "udpserver.h"

#include <QDebug>

UdpServer::UdpServer(const QString &address, uint16_t port, QObject *parent)
    : NetworkObject{parent}, mLocalAddress(address), mLocalPort(port), mUdpServerListener(this)
{
    mUdpServer.Attach(UdpServer_Creator::Create(&mUdpServerListener));
    mUdpServerListener.initNetworkListener();
}

void UdpServer::start()
{
    if (mUdpServer->GetState() <= SS_STARTED) return;
     mUdpServer->Start(LPCTSTR(mLocalAddress.toStdString().data()), mLocalPort);
}

void UdpServer::stop(int32_t dwConnID)
{
    if (dwConnID < 0)
    {
        bool status = mUdpServer->Stop();
        if (!status) return;
        QString token = getBaseToken();
        emit AppSignal::getInstance()->sgl_update_network_object(token);
    }
    else
    {
        mUdpServer->Disconnect(dwConnID);
    }
}

void UdpServer::clear()
{
    // 关闭所有连接
    CONNID pIDs[65535] = { 0 };
    // dwCount 必须赋值，且要大于实际的连接数量才行，否则 GetAllConnectionIDs 返回 false
    DWORD dwCount = 65535;
    bool status = mUdpServer->GetAllConnectionIDs(pIDs, dwCount);
    if (status && (dwCount > 0))
    {
        for (uint64_t i = 0 ; i < dwCount; i++)
        {
            mUdpServer->Disconnect(pIDs[i]);
        }
    }
}

void UdpServer::close()
{
    mUdpServerListener.setActiveStatus(false);
    mUdpServer->Stop();
    deleteLater();

    emit sgl_delete_network_object_finish();
}

void UdpServer::edit(const QString &address, uint16_t port)
{
    mLocalAddress = address;
    mLocalPort = port;
    QString token = getBaseToken();
    emit AppSignal::getInstance()->sgl_update_network_object(token);
}

void UdpServer::send(const std::string &data, uint32_t length, int32_t dwConnID)
{
    mUdpServer->Send(dwConnID, (BYTE*)data.data(), length);
}

NetworkObjectDetail UdpServer::getObjectDetail(int32_t dwConnID)
{
    NetworkObjectDetail detail;
    if (dwConnID < 0)
    {
        detail.type = Udp_Server;
        detail.token = getBaseToken();
        detail.status = mUdpServer->GetState();

        CHAR szAddress[100];
        int iAddressLen = sizeof(szAddress) / sizeof(CHAR);
        USHORT usPort = 0;
        mUdpServer->GetRemoteAddress(dwConnID, (TCHAR*)szAddress, iAddressLen, usPort);

        detail.peerAddress = QString::fromStdString(szAddress);
        detail.peerPort = usPort;
        detail.localAddress = mLocalAddress;
        detail.localPort = mLocalPort;
        detail.name = QString("%1:%2").arg(detail.localAddress, QString::number(detail.localPort));

        detail.clientNumber = mUdpServer->GetConnectionCount();
    }
    else
    {
        detail.type = Udp_Client_Slave;
        detail.token = getBaseToken() + QString::number(dwConnID);
        detail.status = Client_Connected;

        CHAR szAddress[100];
        int iAddressLen = sizeof(szAddress) / sizeof(CHAR);
        USHORT usPort = 0;
        mUdpServer->GetRemoteAddress(dwConnID, (TCHAR*)szAddress, iAddressLen, usPort);

        detail.peerAddress = QString::fromStdString(szAddress);
        detail.peerPort = usPort;

        detail.localAddress = mLocalAddress;
        detail.localPort = mLocalPort;
        detail.name = QString("%1:%2").arg(detail.peerAddress, QString::number(detail.peerPort));
    }

    return detail;
}
