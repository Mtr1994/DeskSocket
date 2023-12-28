#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>

class NetworkManager : public QObject
{
    Q_OBJECT
public:
    explicit NetworkManager(QObject *parent = nullptr);
    NetworkManager(const NetworkManager& signal) = delete;
    NetworkManager& operator=(const NetworkManager& signal) = delete;

public:
    static NetworkManager* getInstance();
    void init();

public slots:
    void slot_prepare_network_object(int type, const QString& address, uint16_t port);
};

#endif // NETWORKMANAGER_H
