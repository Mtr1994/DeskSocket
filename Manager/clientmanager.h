#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include <QObject>
#include <QList>

#include "Public/defines.h"

namespace mtr {

class BaseSocket;
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
    void slot_add_new_client(uint16_t protocol, const QString &ipv4, uint16_t port);
    void slot_client_sent_data(const QString &serverkey, const QString &contentKey, uint64_t dwconnid, const QByteArray &data);
    void slot_delete_client(const QString &serverkey);

    void slot_disconnect_client(const QString &serverkey);
    void slot_connect_client(const QString &serverkey);

private:
    QList<BaseSocket*> mListSocket;
};

}

#endif // CLIENTMANAGER_H
