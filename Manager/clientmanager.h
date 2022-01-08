#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include <QObject>
#include <QList>
#include <QAbstractSocket>

#include "Public/defines.h"

namespace mtr {

class TcpSocket;
class ClientManager : public QObject
{
    Q_OBJECT
private:
    explicit ClientManager(QObject *parent = nullptr);

public:
    static ClientManager* getInstance();

    void init();

public slots:
    // 自定义槽函数
    void slot_add_new_tcp_client(const QString &ip_4, uint16_t port);
    void slot_tcp_client_sent_data(const QString &serverkey, const QString &contentKey, uint64_t dwconnid, const QByteArray &data);
    void slot_delete_tcp_client(const QString &serverkey);

    void slot_disconnect_tcp_client(const QString &serverkey);
    void slot_connect_tcp_client(const QString &serverkey);

signals:
    void sgl_client_operation(int operation, const ClientInfo& info);

private:
    QList<TcpSocket*> mListTcpSocket;
};

}

#endif // CLIENTMANAGER_H
