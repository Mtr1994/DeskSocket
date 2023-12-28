#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QGuiApplication>
#include <QScreen>
#include <QRect>

#include "Log/logger.h"
#include "Public/appsignal.h"
#include "Dialog/dialognetparameter.h"
#include "Public/defines.h"
#include "Network/networkmanager.h"
#include "Control/Message/messagewidget.h"

// test
#include <QDebug>

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
    if (QGuiApplication::screens().size() < 0) return;
    uint16_t baseSize = QGuiApplication::screens().at(0)->physicalDotsPerInch() * 16;
    resize(baseSize * 0.64, baseSize * 0.64 * 0.618);
    LOG_DEBUG(QString("dot pre inch is %1").arg(QString::number(baseSize / 16)).toStdString());

    NetworkManager::getInstance()->init();

    connect(ui->actionAddTcpServer, &QAction::triggered, this, &MainWindow::slot_create_tcp_server);
    connect(ui->actionAddTcpClient, &QAction::triggered, this, &MainWindow::slot_create_tcp_client);
    connect(ui->actionAddUdpServer, &QAction::triggered, this, &MainWindow::slot_create_udp_server);
    connect(ui->actionAddUdpClient, &QAction::triggered, this, &MainWindow::slot_create_udp_client);
    connect(ui->actionAddUDPBroadcast, &QAction::triggered, this, &MainWindow::slot_create_udp_cast_client);
    connect(AppSignal::getInstance(), &AppSignal::sgl_show_system_toast_message, this, &MainWindow::slot_show_system_toast_message);

    connect(ui->actionVersion, &QAction::triggered, this, &MainWindow::slot_show_about_us);

    // 工具窗口
    connect(ui->actionJSONFormat, &QAction::triggered, this, &MainWindow::slot_open_format_json_dialog);

    // 设置窗口
    connect(ui->actionSetting, &QAction::triggered, this, &MainWindow::slot_open_setting_dialog);

    ui->menuNew->setWindowFlag(Qt::NoDropShadowWindowHint);
    ui->menuTool->setWindowFlag(Qt::NoDropShadowWindowHint);
    ui->menuHelp->setWindowFlag(Qt::NoDropShadowWindowHint);
    ui->menuSetting->setWindowFlag(Qt::NoDropShadowWindowHint);
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

void MainWindow::slot_open_setting_dialog()
{
    ui->tpContainer->createCustomContent("Setting");
}

void MainWindow::slot_show_system_toast_message(const QString &msg, int status)
{
    MessageWidget *message = new MessageWidget(status, MessageWidget::P_Top_Center, this);
    message->showMessage("  " + msg);
}
