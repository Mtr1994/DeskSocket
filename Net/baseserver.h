#ifndef BASESERVER_H
#define BASESERVER_H
#include "HPSocket/HPSocket.h"

#include <QObject>

namespace mtr {

class BaseServer : public QObject
{
    Q_OBJECT
public:
    explicit BaseServer(const QString &ip, uint16_t port);

    virtual bool start() = 0;

    virtual void stop() = 0;

    virtual bool restart() = 0;

    virtual void clearClient() = 0;

    virtual bool write(CONNID dwConnID, const QByteArray &data) = 0;

    virtual bool closeSocket(CONNID dwConnID) = 0;

    virtual QString getServerKey() = 0;

private:
    QString mServerAddress;
    int16_t mServerPort;
};

}

#endif // BASESERVER_H
