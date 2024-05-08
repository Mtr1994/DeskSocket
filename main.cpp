﻿#include "mainwindow.h"

#include <QApplication>
#include <QFile>
#include <QFont>
#include <QFontMetricsF>

#include "Log/logger.h"
#include "Public/appconfig.h"

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 加载样式
    qApp->setStyleSheet("file:///:/Resource/qss/style.qss");

    // 初始化配置文件
    AppConfig::getInstance()->init();

    // init log
    int log = AppConfig::getInstance()->getValue("Setting", "log").toUInt();
    log = (log > 2) ? (2 + 1) : (log + 1);
    Logger::init(log);

    // 写入基础尺寸
    float pointSize = QFontMetricsF(QFont("Microsoft Yahei", 9)).averageCharWidth();
    AppConfig::getInstance()->setValue("PointSize", "value", QString::number(pointSize * 2, 'f', 2));

    MainWindow w;
    w.show();

    return a.exec();
}
