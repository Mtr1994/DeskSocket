#include "tcpserver.h"
#include "Log/logger.h"

TcpServer::TcpServer(const QString &address, uint16_t port, QObject *parent)
    : NetworkObject{parent}, mLocalAddress(address), mLocalPort(port), mTcpSocketListener(this)
{
    mTcpPackServer.Attach(TcpServer_Creator::Create(&mTcpSocketListener));
    mTcpSocketListener.initNetworkListener();
}

void TcpServer::start()
{
    LOG_DEBUG("tcp server start {} {}", mLocalAddress.toStdString(), mLocalPort);
    if (mTcpPackServer->GetState() <= SS_STARTED) return;
    mTcpPackServer->Start(LPCTSTR(mLocalAddress.toStdString().data()), mLocalPort);
}

void TcpServer::stop(int32_t dwConnID)
{
    LOG_DEBUG("tcp connect close {}", dwConnID);
    if (dwConnID < 0) mTcpPackServer->Stop();
    else mTcpPackServer->Disconnect(dwConnID);
}

void TcpServer::clear()
{
    // 关闭所有连接
    CONNID pIDs[65535] = { 0 };
    // dwCount 必须赋值，且要大于实际的连接数量才行，否则 GetAllConnectionIDs 返回 false
    DWORD dwCount = 65535;
    bool status = mTcpPackServer->GetAllConnectionIDs(pIDs, dwCount);
    if (status && (dwCount > 0))
    {
        for (uint64_t i = 0 ; i < dwCount; i++)
        {
            mTcpPackServer->Disconnect(pIDs[i]);
        }
    }
}

void TcpServer::close()
{
    LOG_DEBUG("tcp server delete {}", mLocalAddress.toStdString());
    mTcpSocketListener.setActiveStatus(false);
    mTcpPackServer->Stop();
    deleteLater();
    emit sgl_delete_network_object_finish();
}

void TcpServer::edit(const QString &address, uint16_t port)
{
    mLocalAddress = address;
    mLocalPort = port;
    emit AppSignal::getInstance()->sgl_update_network_object(getObjectDetail(-1).token);
}

void TcpServer::send(const std::string &data, uint32_t length, int32_t dwConnID)
{
    mTcpPackServer->Send(dwConnID, (BYTE*)data.data(), length);
}

NetworkObjectDetail TcpServer::getObjectDetail(int32_t dwConnID)
{
    NetworkObjectDetail detail;
    if (dwConnID < 0)
    {
        detail.type = Tcp_Server;
        detail.token = getBaseToken();
        detail.status = mTcpPackServer->GetState();

        CHAR szAddress[100];
        int iAddressLen = sizeof(szAddress) / sizeof(CHAR);
        USHORT usPort = 0;
        mTcpPackServer->GetRemoteAddress(dwConnID, (TCHAR*)szAddress, iAddressLen, usPort);

        detail.peerAddress = QString::fromStdString(szAddress);
        detail.peerPort = usPort;
        detail.localAddress = mLocalAddress;
        detail.localPort = mLocalPort;
        detail.name = QString("%1:%2").arg(detail.localAddress, QString::number(detail.localPort));

        detail.clientNumber = mTcpPackServer->GetConnectionCount();
    }
    else
    {
        detail.type = Tcp_Client_Slave;
        detail.token = getBaseToken() + QString::number(dwConnID);
        detail.status = Client_Connected;

        CHAR szAddress[100];
        int iAddressLen = sizeof(szAddress) / sizeof(CHAR);
        USHORT usPort = 0;
        mTcpPackServer->GetRemoteAddress(dwConnID, (TCHAR*)szAddress, iAddressLen, usPort);

        detail.peerAddress = QString::fromStdString(szAddress);;
        detail.peerPort = usPort;

        detail.localAddress = mLocalAddress;
        detail.localPort = mLocalPort;
        detail.name = QString("%1:%2").arg(detail.peerAddress, QString::number(detail.peerPort));
    }

    return detail;
}
