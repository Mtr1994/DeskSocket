#include "udpbroadcast.h"

UdpBroadcast::UdpBroadcast(const QString &address, uint16_t port, QObject *parent)
    : NetworkObject{parent}, mLocalAddress(address), mLocalPort(port), mUdpBroadcastListener(this)
{
    mUdpCastPtr.Attach(UdpCast_Creator::Create(&mUdpBroadcastListener));
    mUdpCastPtr->SetCastMode(CM_BROADCAST);
    mUdpBroadcastListener.initNetworkListener();
}

void UdpBroadcast::start()
{
    if (mUdpCastPtr->GetState() <= SS_STARTED) return;
    mUdpCastPtr->Start(LPCTSTR(mLocalAddress.toStdString().data()), mLocalPort, false);
}

void UdpBroadcast::stop(int32_t dwConnID)
{
    Q_UNUSED(dwConnID);
    bool status = mUdpCastPtr->Stop();
    if (!status) return;
    QString token = getBaseToken();
    emit AppSignal::getInstance()->sgl_update_network_object(token);
}

void UdpBroadcast::clear()
{
    // nothing to do here
}

void UdpBroadcast::close()
{
    mUdpBroadcastListener.setActiveStatus(false);
    mUdpCastPtr->Stop();
    deleteLater();

    emit sgl_delete_network_object_finish();
}

void UdpBroadcast::edit(const QString &address, uint16_t port)
{
    mLocalAddress = address;
    mLocalPort = port;
    QString token = getBaseToken();
    emit AppSignal::getInstance()->sgl_update_network_object(token);
}

void UdpBroadcast::send(const std::string &data, uint32_t length, int32_t dwConnID)
{
    Q_UNUSED(dwConnID);
    if (!mUdpCastPtr.IsValid()) return;
    mUdpCastPtr->Send((BYTE*)data.data(), (int)length);
}

NetworkObjectDetail UdpBroadcast::getObjectDetail(int32_t dwConnID)
{
    Q_UNUSED(dwConnID);

    NetworkObjectDetail detail;
    detail.type = Udp_Client;
    detail.name = QString("%1:%2").arg(mLocalAddress, QString::number(mLocalPort));
    detail.token = getBaseToken();
    detail.status = mUdpCastPtr->GetState();

    detail.peerAddress = mLocalAddress;
    detail.peerPort = mLocalPort;

    CHAR szAddress[100];
    int iAddressLen = sizeof(szAddress) / sizeof(CHAR);
    USHORT usPort = 0;
    mUdpCastPtr->GetLocalAddress((TCHAR*)szAddress, iAddressLen, usPort);

    detail.localAddress = QString::fromStdString(szAddress);;
    detail.localPort = usPort;

    return detail;
}

