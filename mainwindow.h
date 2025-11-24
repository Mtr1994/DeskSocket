#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void init();

private slots:
    void slot_create_tcp_server();

    void slot_create_tcp_client();

    void slot_create_udp_server();

    void slot_create_udp_client();

    void slot_create_udp_cast_client();

    void slot_show_about_us();

    void slot_open_format_json_dialog();

    void slot_open_format_pos_dialog();

    void slot_open_setting_dialog();

    void slot_auto_update_dialog();

    void slot_show_system_toast_message(const QString &msg, int status);

    void slot_edit_network_object(const QString &address, const QString &port, const QString &token);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
