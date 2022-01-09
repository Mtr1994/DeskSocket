#include "mainwindow.h"

#include <QApplication>
#include <QFile>

#include "Public/defines.h"
#include "Net/tcpsocket.h"
#include "Log/logger.h"

using namespace std;
using namespace mtr;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 加载样式
    QFile qss(":/resources/qss/style.qss");
    qss.open(QFile::ReadOnly);
    qApp->setStyleSheet(qss.readAll());
    qss.close();

    // 初始化日志
    Logger logger;
    logger.init();
    LOG_DEBUG("start up");

    MainWindow w;
    w.show();
    return a.exec();
}
