#include "tcpserver.h"
#include "tcpsocket.h"
#include "Log/logger.h"

using namespace mtr;

TcpServer::TcpServer(const QString &ip, uint32_t port)
    : BaseServer(ip, port), mIPV4(ip), mPort(port)
{

}

TcpServer::~TcpServer()
{
    LOG_DEBUG("delete tcpserver");
}

bool TcpServer::start()
{
    mTcpPackServer.Attach(TcpServer_Creator::Create(&mTcpSocketListener));
    return mTcpPackServer->Start(LPCTSTR(mIPV4.toStdWString().data()), mPort);
}

void TcpServer::stop()
{
    mTcpPackServer->Stop();
}

bool TcpServer::restart()
{
    return mTcpPackServer->Start(LPCTSTR(mIPV4.toStdWString().data()), mPort);
}

void TcpServer::clearClient()
{
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

QString TcpServer::getServerKey()
{
    return QString("TCPSERVER:%1:%2").arg(mIPV4, QString::number(mPort));
}

bool TcpServer::write(CONNID dwConnID, const std::string &data)
{
    return  mTcpPackServer->Send(dwConnID, (BYTE*)data.data(), data.length());
}

bool TcpServer::closeSocket(CONNID dwConnID)
{
    return mTcpPackServer->Disconnect(dwConnID);
}
