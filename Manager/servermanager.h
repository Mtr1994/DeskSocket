#ifndef SERVERMANAGER_H
#define SERVERMANAGER_H

#include <QObject>
#include <QList>

namespace mtr {

class TcpServer;
class ServerManager : public QObject
{
    Q_OBJECT
private:
    explicit ServerManager(QObject *parent = nullptr);

public:
    static ServerManager* getInstance();

    void init();

public slots:
    void slot_add_new_tcp_server(const QString &ip_4, uint16_t port);

    void slot_slave_tcp_client_sent_data(const QString &serverkey, const QString &contentKey, uint64_t dwconnid, const QByteArray &data);
    void slot_delete_slave_tcp_client(const QString &serverkey, uint64_t dwconnid);

    void slot_stop_tcp_server(const QString &serverkey);
    void slot_start_tcp_server(const QString &serverkey);

    void slot_close_all_slave_tcp_client(const QString &serverkey);

    void slot_delete_tcp_server(const QString &serverkey);

private:
    QList<TcpServer*> mListTcpServer;
};

}

#endif // SERVERMANAGER_H
