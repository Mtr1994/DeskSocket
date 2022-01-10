#include "tcpsocket.h"

#include <QHostAddress>

using namespace mtr;

TcpSocket::TcpSocket()
{

}

bool TcpSocket::connect(const QString &ipv4, uint16_t port)
{
    mServerAddress = ipv4;
    mServerPort = port;
    mTcpClientPtr.Attach(TcpClient_Creator::Create(&mTcpClientListener));
    return mTcpClientPtr->Start(LPCTSTR(ipv4.toStdWString().data()), port);
}

bool TcpSocket::connect()
{
    return mTcpClientPtr->Start(LPCTSTR(mServerAddress.toStdWString().data()), mServerPort);
}

QString TcpSocket::getServerKey()
{
    return QString("TCPCLIENT:%1:%2").arg(mServerAddress, QString::number(mServerPort));
}

bool TcpSocket::write(const QByteArray &data)
{
    return mTcpClientPtr->Send((BYTE*)data.data(), data.length());
}

bool TcpSocket::closeSocket()
{
    return mTcpClientPtr->Stop();
}
