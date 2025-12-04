#include "udpsocket.h"

UdpSocket::UdpSocket(const QString &address, uint16_t port, QObject *parent)
    : NetworkObject{parent}, mPeerAddress(address), mPeerPort(port), mUdpClientListener(this)
{
    mUdpClientPtr.Attach(UdpClient_Creator::Create(&mUdpClientListener));
    mUdpClientListener.initNetworkListener();
}

void UdpSocket::start()
{
    if (mUdpClientPtr->GetState() <= SS_STARTED) return;
    mUdpClientPtr->Start(LPCTSTR(mPeerAddress.toStdString().data()), mPeerPort);
}

void UdpSocket::stop(int32_t dwConnID)
{
    Q_UNUSED(dwConnID);
    bool status = mUdpClientPtr->Stop();
    if (!status) return;
    QString token = getBaseToken();
    emit AppSignal::getInstance()->sgl_update_network_object(token);
}

void UdpSocket::clear()
{
    // nothing to do here
}

void UdpSocket::close()
{
    mUdpClientListener.setActiveStatus(false);
    mUdpClientPtr->Stop();
    deleteLater();

    emit sgl_delete_network_object_finish();
}

void UdpSocket::edit(const QString &address, uint16_t port)
{
    mPeerAddress = address;
    mPeerPort = port;
    QString token = getBaseToken();
    emit AppSignal::getInstance()->sgl_update_network_object(token);
}

void UdpSocket::send(const std::string &data, uint32_t length, int32_t dwConnID)
{
    Q_UNUSED(dwConnID);
    if (!mUdpClientPtr.IsValid()) return;
    mUdpClientPtr->Send((BYTE*)data.data(), (int)length);
}

NetworkObjectDetail UdpSocket::getObjectDetail(int32_t dwConnID)
{
    Q_UNUSED(dwConnID);

    NetworkObjectDetail detail;
    detail.type = Udp_Client;
    detail.name = QString("%1:%2").arg(mPeerAddress, QString::number(mPeerPort));
    detail.token = getBaseToken();
    detail.status = mUdpClientPtr->GetState();

    detail.peerAddress = mPeerAddress;
    detail.peerPort = mPeerPort;

    CHAR szAddress[100];
    int iAddressLen = sizeof(szAddress) / sizeof(CHAR);
    USHORT usPort = 0;
    mUdpClientPtr->GetLocalAddress((TCHAR*)szAddress, iAddressLen, usPort);

    detail.localAddress = QString::fromStdString(szAddress);;
    detail.localPort = usPort;

    return detail;
}
