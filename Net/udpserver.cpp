#include "udpserver.h"

using namespace mtr;

UdpServer::UdpServer(const QString &ip, uint16_t port)
    : BaseServer{ip, port}, mServerAddress(ip), mServerPort(port)
{

}

bool UdpServer::start()
{
    mUdpServer.Attach(UdpServer_Creator::Create(&mUdpServerListener));
    return mUdpServer->Start(LPCTSTR(mServerAddress.toStdWString().data()), mServerPort);
}

void UdpServer::stop()
{
    mUdpServer->Stop();
}

bool UdpServer::restart()
{
    return mUdpServer->Start(LPCTSTR(mServerAddress.toStdWString().data()), mServerPort);
}

void UdpServer::clearClient()
{

}

bool UdpServer::write(CONNID dwConnID, const QByteArray &data)
{
    return  mUdpServer->Send(dwConnID, (BYTE*)data.data(), data.length());
}

bool UdpServer::closeSocket(CONNID dwConnID)
{
    return mUdpServer->Disconnect(dwConnID);
}

QString UdpServer::getServerKey()
{
    return QString("UDPSERVER:%1:%2").arg(mServerAddress, QString::number(mServerPort));
}
