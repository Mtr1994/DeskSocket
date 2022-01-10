#include "udpbroadcast.h"

using namespace mtr;

UdpBroadcast::UdpBroadcast()
{

}

bool UdpBroadcast::connect(const QString &ipv4, uint16_t port)
{
    mServerAddress = "255.255.255.255";
    mServerPort = port;
    mUdpCastPtr.Attach(UdpCast_Creator::Create(&mUdpBroadcastListener));
    mUdpCastPtr->SetCastMode(CM_BROADCAST);

    return mUdpCastPtr->Start(LPCTSTR(L"255.255.255.255"), port, false, LPCTSTR(L"0.0.0.0"));
}

bool UdpBroadcast::connect()
{
    return mUdpCastPtr->Start(LPCTSTR(mServerAddress.toStdWString().data()), mServerPort);
}

QString UdpBroadcast::getServerKey()
{
    return QString("UDPCLIENT:%1:%2").arg(mServerAddress, QString::number(mServerPort));
}

bool UdpBroadcast::write(const QByteArray &data)
{
    return mUdpCastPtr->Send((BYTE*)data.data(), data.length());
}

bool UdpBroadcast::closeSocket()
{
    return mUdpCastPtr->Stop();
}
