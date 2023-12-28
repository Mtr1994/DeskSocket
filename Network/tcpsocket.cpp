#include "tcpsocket.h"

// test
#include <QDebug>
#include <QThread>

TcpSocket::TcpSocket(const QString &address, uint16_t port, QObject *parent)
    : NetworkObject{parent}, mPeerAddress(address), mPeerPort(port), mTcpClientListener(this)
{
    mTcpClientPtr.Attach(TcpClient_Creator::Create(&mTcpClientListener));
    mTcpClientListener.initNetworkListener();
}

void TcpSocket::start()
{
    if (mTcpClientPtr->GetState() <= SS_STARTED) return;
    mTcpClientPtr->Start(LPCTSTR(mPeerAddress.toStdString().data()), mPeerPort);
    qDebug() << "TcpSocket::start" << QThread::currentThreadId();
}

void TcpSocket::stop(int32_t dwConnID)
{
    Q_UNUSED(dwConnID);
    mTcpClientPtr->Stop();
}

void TcpSocket::clear()
{
    // nothing to do here
}

void TcpSocket::close()
{
    mTcpClientListener.setActiveStatus(false);
    mTcpClientPtr->Stop();
    deleteLater();

    emit sgl_delete_network_object_finish();
}

void TcpSocket::send(const std::string &data, uint32_t length, int32_t dwConnID)
{
    Q_UNUSED(dwConnID);
    //qDebug() << "token " << getBaseToken();
    //qDebug() << "socket th " << QThread::currentThreadId() << length;
    if (!mTcpClientPtr.IsValid()) return;
    mTcpClientPtr->Send((BYTE*)data.data(), (int)length);
}

NetworkObjectDetail TcpSocket::getObjectDetail(int32_t dwConnID)
{
    Q_UNUSED(dwConnID);

    NetworkObjectDetail detail;
    detail.type = Tcp_Client;
    detail.name = QString("%1:%2").arg(mPeerAddress, QString::number(mPeerPort));
    detail.token = getBaseToken();
    detail.status = mTcpClientPtr->GetState();
    detail.peerAddress = mPeerAddress;
    detail.peerPort = mPeerPort;

    CHAR szAddress[100];
    int iAddressLen = sizeof(szAddress) / sizeof(CHAR);
    USHORT usPort = 0;
    mTcpClientPtr->GetLocalAddress((TCHAR*)szAddress, iAddressLen, usPort);

    detail.localAddress = QString::fromStdString(szAddress);;
    detail.localPort = usPort;

    return detail;
}

