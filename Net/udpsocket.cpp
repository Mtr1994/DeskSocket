#include "udpsocket.h"

using namespace mtr;

UdpSocket::UdpSocket()
{

}

bool mtr::UdpSocket::connect(const QString &ipv4, uint16_t port)
{
    mServerAddress = ipv4;
    mServerPort = port;
    mUdpClientPtr.Attach(UdpClient_Creator::Create(&mUdpClientListener));
    return mUdpClientPtr->Start(LPCTSTR(ipv4.toStdWString().data()), port);
}

bool mtr::UdpSocket::connect()
{
    return mUdpClientPtr->Start(LPCTSTR(mServerAddress.toStdWString().data()), mServerPort);
}

QString mtr::UdpSocket::getServerKey()
{
    return QString("UDPCLIENT:%1:%2").arg(mServerAddress, QString::number(mServerPort));
}

bool mtr::UdpSocket::write(const std::string &data)
{
    return mUdpClientPtr->Send((BYTE*)data.data(), data.length());
}

bool mtr::UdpSocket::closeSocket()
{
    return mUdpClientPtr->Stop();
}
