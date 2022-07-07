#ifndef BASESOCKET_H
#define BASESOCKET_H

#include <QObject>

namespace mtr {

class BaseSocket : public QObject
{
    Q_OBJECT
public:
    explicit BaseSocket(QObject *parent = nullptr);

    virtual bool connect(const QString &ipv4, uint16_t port) = 0;
    virtual bool connect() = 0;
    virtual QString getServerKey() = 0;
    virtual bool write(const std::string &data) = 0;
    virtual bool closeSocket() = 0;

private:
    QString mServerAddress;
    uint16_t mServerPort;
};

}

#endif // BASESOCKET_H
