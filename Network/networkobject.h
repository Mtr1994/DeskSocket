#ifndef NETWORKOBJECT_H
#define NETWORKOBJECT_H

#include "Public/defines.h"

#include <QObject>

class NetworkObject : public QObject
{
    Q_OBJECT
public:
    explicit NetworkObject(QObject *parent = nullptr);

private:
    void init();

protected:
    QString getBaseToken() const;

signals:
    void sgl_delete_network_object_finish();

public:
    virtual void start() = 0;
    virtual void stop(int32_t dwConnID) = 0;
    virtual void clear() = 0;
    virtual void close() = 0;
    virtual void edit(const QString &address, uint16_t port) = 0;

    virtual void send(const std::string &data, uint32_t length, int32_t dwConnID = -1) = 0;
    virtual NetworkObjectDetail getObjectDetail(int32_t dwConnID = -1) = 0;

public slots:
    void slot_init_network_object();

    void slot_start_network_object(const QString& token);
    void slot_stop_network_object(const QString& token, int32_t dwConnID);
    void slot_clear_network_object(const QString& token);
    void slot_delete_network_object(const QString& token);
    void slot_recreate_network_object(const QString &token, const QString &address, uint16_t port);

private:
    QString mBaseToken;
    int mNetworkType = -1;
};

#endif // NETWORKOBJECT_H
