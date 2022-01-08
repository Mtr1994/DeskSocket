#ifndef APPSIGNAL_H
#define APPSIGNAL_H

#include <QObject>

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
    // 打开新增 TCP 服务端窗口
    void sgl_open_add_tcp_server_dialog();

    // 新增一个 TCP 服务
    void sgl_add_new_tcp_server(const QString &ip_4, uint16_t port);

    // TCP 服务监听成功
    void sgl_tcp_server_status_change(const QString &ip_4, uint16_t port, int status);

    // 接受一个 TCP 客户端链接
    void sgl_recv_new_slave_tcp_client(const QString &key, const QString &ip_4, uint16_t port, uint64_t dwconnid);

    // 收到 TCP 客户端数据
    void sgl_thread_recv_slave_tcp_client_data(const QString &key, const QString &ip_4, uint16_t port, uint64_t dwconnid, const QByteArray &data);

    // 接受的 TCP 客户端发送数据
    void sgl_slave_tcp_client_sent_data(const QString &serverkey, const QString &contentKey, uint64_t dwconnid, const QByteArray &data);

    // 接受的 TCP 客户端发送数据结果
    void sgl_slave_tcp_client_sent_data_result(const QString &contentKey, bool status, uint32_t length, const QString &error);

    // 关闭接受的 TCP 客户端连接
    void sgl_delete_slave_tcp_client(const QString &serverkey, uint64_t dwconnid);

    // 关闭接受的 TCP 客户端连结果
    void sgl_close_slave_tcp_client_result(const QString &socketkey);

    // 关闭 TCP 服务
    void sgl_stop_tcp_server(const QString &serverkey);

    // 启动 TCP 服务
    void sgl_start_tcp_server(const QString &serverkey);

    // 关闭所有被动 TCP 连接
    void sgl_close_all_slave_tcp_client(const QString &serverkey);

    // 删除 TCP 服务
    void sgl_delete_tcp_server(const QString &serverkey);

    // 删除 TCP 服务完成
    void sgl_delete_tcp_server_finish(const QString &serverkey);

    // 打开新增 TCP 客户端窗口
    void sgl_open_add_tcp_client_dialog();

    // 新增一个 TCP 客户端链接
    void sgl_add_new_tcp_client(const QString &ip_4, uint16_t port);

    // TCP 客户端连接（主动）状态改变
    void sgl_tcp_client_connected(const QString &key, const QString &ip_4, uint16_t port, uint64_t dwconnid);

    // 新增一个 TCP TAB 页面
    void sgl_add_tcp_tab_page(const QString &flag, const QString &key, const QString &ip_4, uint16_t port, uint64_t dwconnid);

    // 收到 TCP 客户端（主动）数据
    void sgl_thread_recv_tcp_client_data(const QString &key, const QString &ip_4, uint16_t port, uint64_t dwconnid, const QByteArray &data);

    // TCP 客户端（主动）发送数据
    void sgl_tcp_client_sent_data(const QString &serverkey, const QString &contentKey, uint64_t dwconnid, const QByteArray &data);

    // TCP 客户端（主动）发送数据结果
    void sgl_tcp_client_sent_data_result(const QString &contentKey, bool status, uint32_t length, const QString &error);

    // 关闭 TCP 连接
    void sgl_disconnect_tcp_client(const QString &serverkey);

    // 开启 TCP 连接
    void sgl_connect_tcp_client(const QString &serverkey);

    // 删除 TCP 客户端（主动）
    void sgl_delete_tcp_client(const QString &serverkey);

    // 删除 TCP 客户端（主动）完成
    void sgl_delete_tcp_client_finish(const QString &serverkey, bool status);

    // 接受的 TCP 客户端断开
    void sgl_tcp_client_closed(const QString &socketkey);

    // 销毁一个 TCP TAB 页面
    void sgl_delete_tcp_tab_page(uint64_t socketptr);

    // 当前选中的 Socket 发生变化
    void sgl_current_socket_index_change(const QString &socketkey);
};

#endif // APPSIGNAL_H
