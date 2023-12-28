#include "mainwindow.h"

#include <QApplication>
#include <QFile>

#include "Log/logger.h"
#include "Public/appconfig.h"

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 加载样式
    qApp->setStyleSheet("file:///:/Resource/qss/style.qss");

    // 初始化日志
    Logger logger;
    logger.init();

    // 初始化配置文件
    AppConfig::getInstance()->init();

    MainWindow w;
    w.show();

    return a.exec();
}
