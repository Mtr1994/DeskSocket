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

    // 新增一个 TCP/UDP 服务
    void sgl_add_new_server(uint16_t protocol, const QString &ip_4, uint16_t port);

    // 服务监听成功
    void sgl_server_status_change(uint16_t protocol, const QString &ip_4, uint16_t port, int status);

    // 接受一个客户端连接
    void sgl_recv_new_slave_client(uint16_t protocol, const QString &serverkey, const QString &ip_4, uint16_t port, uint64_t dwconnid);

    // 接受一个客户端数据
    void sgl_thread_recv_slave_client_data(const QString &key, const QString &ip_4, uint16_t port, uint64_t dwconnid, const std::string &data);

    // 接受的客户端发送数据
    void sgl_slave_client_sent_data(const QString &serverkey, const QString &contentKey, uint64_t dwconnid, const std::string &data);

    // 接受的客户端发送数据结果
    void sgl_slave_client_sent_data_result(const QString &contentKey, bool status, uint32_t length, const QString &error);

    // 关闭接受的客户端连接
    void sgl_delete_slave_client(const QString &serverkey, uint64_t dwconnid);

    // 关闭接受的客户端连结果
    void sgl_close_slave_client_result(const QString &socketkey);

    // 关闭服务
    void sgl_stop_server(const QString &serverkey);

    // 启动服务
    void sgl_restart_server(const QString &serverkey);

    // 关闭所有被动连接
    void sgl_close_all_slave_client(const QString &serverkey);

    // 删除服务
    void sgl_delete_server(const QString &serverkey);

    // 删除服务完成
    void sgl_delete_server_finish(const QString &serverkey);

    // 打开新增 TCP 客户端窗口
    void sgl_open_add_tcp_client_dialog();

    // 新增一个客户端链接
    void sgl_add_new_client(uint16_t protocol, const QString &ip_4, uint16_t port);

    // 客户端连接（主动）状态改变
    void sgl_client_connected(uint16_t protocol, const QString &serverkey, const QString &ip_4, uint16_t port, uint64_t dwconnid);

    // 新增一个 TAB 页面
    void sgl_add_tab_page(uint16_t protocol, const QString &flag, const QString &key, const QString &ip_4, uint16_t port, uint64_t dwconnid);

    // 收到客户端（主动）数据
    void sgl_thread_recv_client_data(const QString &key, const QString &ip_4, uint16_t port, uint64_t dwconnid, const std::string &data);

    // 客户端（主动）发送数据
    void sgl_client_sent_data(const QString &serverkey, const QString &contentKey, uint64_t dwconnid, const std::string &data);

    // 客户端（主动）发送数据结果
    void sgl_client_sent_data_result(const QString &contentKey, bool status, uint32_t length, const QString &error);

    // 关闭连接
    void sgl_disconnect_client(const QString &serverkey);

    // 开启连接
    void sgl_reconnect_client(const QString &serverkey);

    // 删除客户端（主动）
    void sgl_delete_client(const QString &serverkey);

    // 删除客户端（主动）完成
    void sgl_delete_client_finish(const QString &serverkey, bool status);

    // 接受的客户端断开
    void sgl_client_closed(const QString &socketkey);

    // 销毁一个 TAB 页面
    void sgl_delete_tab_page(uint16_t protocol, uint64_t socketptr);

    // 当前选中的 Socket 发生变化
    void sgl_current_socket_index_change(const QString &socketkey);

    //////////////////////// UDP ////////////////////////////////////////
    // 打开新增 UDP 服务端窗口
    void sgl_open_add_udp_server_dialog();

    void sgl_open_add_udp_client_dialog();

    void sgl_open_add_udp_cast_client_dialog();
    // 打开一个广播 客户端

    /// //////////////////////// UDP ////////////////////////////////////////
};

#endif // APPSIGNAL_H
