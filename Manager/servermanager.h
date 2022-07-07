#ifndef SERVERMANAGER_H
#define SERVERMANAGER_H

#include <QObject>
#include <QList>

namespace mtr {

class BaseServer;
class UdpServer;
class ServerManager : public QObject
{
    Q_OBJECT
private:
    explicit ServerManager(QObject *parent = nullptr);

public:
    static ServerManager* getInstance();

    void init();

public slots:
    void slot_add_new_server(uint16_t protocol, const QString &ipv4, uint16_t port);

    void slot_slave_client_sent_data(const QString &serverkey, const QString &contentKey, uint64_t dwconnid, const std::string &data);

    void slot_delete_slave_client(const QString &serverkey, uint64_t dwconnid);

    void slot_stop_server(const QString &serverkey);

    void slot_restart_server(const QString &serverkey);

    void slot_close_all_slaveclient(const QString &serverkey);

    void slot_delete_server(const QString &serverkey);

private:
    QList<BaseServer*> mListServer;
};

}

#endif // SERVERMANAGER_H
