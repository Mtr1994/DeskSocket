#ifndef APPSIGNAL_H
#define APPSIGNAL_H

#include <QObject>

#include "Network/networkobject.h"

class AppSignal : public QObject
{
    Q_OBJECT
private:
    explicit AppSignal(QObject *parent = nullptr);
    AppSignal(const AppSignal& signal) = delete;
    AppSignal& operator=(const AppSignal& signal) = delete;

public:
    static AppSignal* getInstance();

signals:
    void sgl_create_network_object(int type, const QString& address, uint16_t port);

    void sgl_recreate_network_object(const QString &token, const QString &address, uint16_t port);

    void sgl_create_network_object_finish(NetworkObject* netObj);

    void sgl_recv_new_network_object(NetworkObject* netObj, uint16_t dwConnID);

    void sgl_recv_network_data(const QString& token, const std::string& data, uint16_t length);

    void sgl_start_network_object(const QString& token);

    void sgl_clear_network_object(const QString& token);

    void sgl_edit_network_object(const QString &address, const QString &port, const QString &token);

    void sgl_stop_network_object(const QString& token, int32_t dwConnID = -1);

    void sgl_delete_network_object(const QString& token);

    void sgl_update_network_object(const QString& token);

    void sgl_current_network_object_change(const QString& token);

    void sgl_show_system_toast_message(const QString &msg, int status);

};

#endif // APPSIGNAL_H
