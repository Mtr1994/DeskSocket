#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QGuiApplication>
#include <QScreen>
#include <QRect>

#include "Log/logger.h"
#include "Dialog/dialogabout.h"
#include "Public/appsignal.h"

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
    QRect rect = QGuiApplication::screens().at(0)->availableGeometry();
    resize(rect.width() * 0.52, rect.height() * 0.6);

    ui->splitter->setStretchFactor(0, 2);
    ui->splitter->setStretchFactor(1, 9);

    connect(ui->actionAddTcpServer, &QAction::triggered, AppSignal::getInstance(), &AppSignal::sgl_open_add_tcp_server_dialog);
    connect(ui->actionAddTcpClient, &QAction::triggered, AppSignal::getInstance(), &AppSignal::sgl_open_add_tcp_client_dialog);
    connect(ui->actionAddUdpServer, &QAction::triggered, AppSignal::getInstance(), &AppSignal::sgl_open_add_udp_server_dialog);
    connect(ui->actionAddUdpClient, &QAction::triggered, AppSignal::getInstance(), &AppSignal::sgl_open_add_udp_client_dialog);
    connect(ui->actionAddUDPBroadcast, &QAction::triggered, AppSignal::getInstance(), &AppSignal::sgl_open_add_udp_cast_client_dialog);

    connect(ui->actionVersion, &QAction::triggered, this, &MainWindow::slot_show_about_us);
}

void MainWindow::slot_show_about_us()
{
    DialogAbout about(this);
    about.exec();
}
