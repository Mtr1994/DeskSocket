#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QGuiApplication>
#include <QScreen>
#include <QRect>

#include "Public/appsignal.h"
#include "Dialog/dialognetparameter.h"
#include "Public/defines.h"
#include "Network/networkmanager.h"
#include "Control/Message/messagewidget.h"
#include "Public/appconfig.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    init();

    setWindowTitle("木头人网络测试工具");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    QString geo = AppConfig::getInstance()->getValue("Main", "Geometry");
    restoreGeometry(QByteArray::fromHex(geo.toStdString().data()));
    NetworkManager::getInstance()->init();

    connect(ui->actionAddTcpServer, &QAction::triggered, this, &MainWindow::slot_create_tcp_server);
    connect(ui->actionAddTcpClient, &QAction::triggered, this, &MainWindow::slot_create_tcp_client);
    connect(ui->actionAddUdpServer, &QAction::triggered, this, &MainWindow::slot_create_udp_server);
    connect(ui->actionAddUdpClient, &QAction::triggered, this, &MainWindow::slot_create_udp_client);
    connect(ui->actionAddUDPBroadcast, &QAction::triggered, this, &MainWindow::slot_create_udp_cast_client);
    connect(AppSignal::getInstance(), &AppSignal::sgl_show_system_toast_message, this, &MainWindow::slot_show_system_toast_message);
    connect(AppSignal::getInstance(), &AppSignal::sgl_edit_network_object, this, &MainWindow::slot_edit_network_object);

    connect(ui->actionVersion, &QAction::triggered, this, &MainWindow::slot_show_about_us);

    // 工具窗口
    connect(ui->actionJSONFormat, &QAction::triggered, this, &MainWindow::slot_open_format_json_dialog);
    connect(ui->actionFormatPos, &QAction::triggered, this, &MainWindow::slot_open_format_pos_dialog);

    // 设置窗口
    connect(ui->actionSetting, &QAction::triggered, this, &MainWindow::slot_open_setting_dialog);

    for (auto &obj : ui->menubar->children())
    {
        QMenu *menu = dynamic_cast<QMenu *>(obj);
        if (nullptr == menu) continue;
        menu->setWindowFlags(Qt::NoDropShadowWindowHint | Qt::Popup | Qt::FramelessWindowHint);
        menu->setAttribute(Qt::WA_TranslucentBackground);
    }

    // 软件更新
    connect(ui->actionAutoUpdate, &QAction::triggered, this, &MainWindow::slot_auto_update_dialog);
}

void MainWindow::slot_create_tcp_server()
{
    DialogNetParameter dialog(Tcp_Server, "TCP 服务端参数", this);
    dialog.exec();
}

void MainWindow::slot_create_tcp_client()
{
    DialogNetParameter dialog(Tcp_Client, "TCP 客户端参数", this);
    dialog.exec();
}

void MainWindow::slot_create_udp_server()
{
    DialogNetParameter dialog(Udp_Server, "UDP 服务端参数", this);
    dialog.exec();
}

void MainWindow::slot_create_udp_client()
{
    DialogNetParameter dialog(Udp_Client, "UDP 客户端参数", this);
    dialog.exec();
}

void MainWindow::slot_create_udp_cast_client()
{
    DialogNetParameter dialog(Udp_Client_Cast, "UDP 广播端参数", this);
    dialog.exec();
}

void MainWindow::slot_show_about_us()
{
    ui->tpContainer->createCustomContent("Version");
}

void MainWindow::slot_open_format_json_dialog()
{
    ui->tpContainer->createCustomContent("FormatJson");
}

void MainWindow::slot_open_format_pos_dialog()
{
    ui->tpContainer->createCustomContent("FormatPos");
}

void MainWindow::slot_open_setting_dialog()
{
    ui->tpContainer->createCustomContent("Setting");
}

void MainWindow::slot_auto_update_dialog()
{
    return;
    close();
    exit(3);
}

void MainWindow::slot_show_system_toast_message(const QString &msg, int status)
{
    MessageWidget *message = new MessageWidget(status, MessageWidget::P_Top_Center, this);
    message->showMessage("  " + msg);
}

void MainWindow::slot_edit_network_object(const QString &address, const QString &port, const QString &token)
{
    DialogNetParameter dialog(address, port, token, this);
    dialog.exec();
}
