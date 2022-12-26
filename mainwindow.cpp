#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QGuiApplication>
#include <QScreen>
#include <QRect>

#include "Log/logger.h"
#include "Dialog/dialogabout.h"
#include "Public/appsignal.h"
#include "Dialog/dialogformatjson.h"

using namespace mtr;

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
    qRegisterMetaType<uint16_t>("uint16_t");
    qRegisterMetaType<uint64_t>("uint64_t");
    qRegisterMetaType<std::string>("std::string");

    QRect rect = QGuiApplication::screens().at(0)->availableGeometry();
    resize(rect.width() * 0.48, rect.width() * 0.48 * 0.618);

    connect(ui->actionAddTcpServer, &QAction::triggered, AppSignal::getInstance(), &AppSignal::sgl_open_add_tcp_server_dialog);
    connect(ui->actionAddTcpClient, &QAction::triggered, AppSignal::getInstance(), &AppSignal::sgl_open_add_tcp_client_dialog);
    connect(ui->actionAddUdpServer, &QAction::triggered, AppSignal::getInstance(), &AppSignal::sgl_open_add_udp_server_dialog);
    connect(ui->actionAddUdpClient, &QAction::triggered, AppSignal::getInstance(), &AppSignal::sgl_open_add_udp_client_dialog);
    connect(ui->actionAddUDPBroadcast, &QAction::triggered, AppSignal::getInstance(), &AppSignal::sgl_open_add_udp_cast_client_dialog);

    connect(ui->actionVersion, &QAction::triggered, this, &MainWindow::slot_show_about_us);

    // 工具窗口
    connect(ui->actionJSONFormat, &QAction::triggered, this, &MainWindow::slot_open_format_json_dialog);

    ui->menuNew->setWindowFlag(Qt::NoDropShadowWindowHint);
    ui->menuTool->setWindowFlag(Qt::NoDropShadowWindowHint);
    ui->menuHelp->setWindowFlag(Qt::NoDropShadowWindowHint);
}

void MainWindow::slot_show_about_us()
{
    DialogAbout about(this);
    about.exec();
}

void MainWindow::slot_open_format_json_dialog()
{
    DialogFormatJson dialog(this);
    dialog.exec();
}
