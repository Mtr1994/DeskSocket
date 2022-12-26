#include "widgettabpages.h"
#include "ui_widgettabpages.h"

#include <QTabBar>
#include <QTextCodec>

#include "Widget/widgettabcontent.h"
#include "Public/appsignal.h"
#include "Log/logger.h"

// test
#include <QDebug>

WidgetTabPages::WidgetTabPages(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetTabPages)
{
    ui->setupUi(this);

    init();
}

WidgetTabPages::~WidgetTabPages()
{
    delete ui;
}

void WidgetTabPages::init()
{
    ui->tabWidget->tabBar()->hide();

    connect(AppSignal::getInstance(), &AppSignal::sgl_add_tab_page, this, &WidgetTabPages::slot_add_tab_page);
    connect(AppSignal::getInstance(), &AppSignal::sgl_thread_recv_slave_client_data, this, &WidgetTabPages::slot_thread_recv_slave_client_data, Qt::QueuedConnection);
    connect(AppSignal::getInstance(), &AppSignal::sgl_current_socket_index_change, this, &WidgetTabPages::slot_current_socket_index_change);
    connect(AppSignal::getInstance(), &AppSignal::sgl_slave_client_sent_data_result, this, &WidgetTabPages::slot_slave_client_sent_data_result);
    connect(AppSignal::getInstance(), &AppSignal::sgl_close_slave_client_result, this, &WidgetTabPages::slot_close_slave_client_result);
    connect(AppSignal::getInstance(), &AppSignal::sgl_thread_recv_client_data, this, &WidgetTabPages::slot_thread_recv_client_data);
    connect(AppSignal::getInstance(), &AppSignal::sgl_client_sent_data_result, this, &WidgetTabPages::slot_client_sent_data_result);
    connect(AppSignal::getInstance(), &AppSignal::sgl_client_closed, this, &WidgetTabPages::slot_client_closed);
}

int WidgetTabPages::getTabCount()
{
    return ui->tabWidget->count();
}

void WidgetTabPages::slot_add_tab_page(uint16_t protocol, const QString &flag, const QString &key, const QString &ip_4, uint16_t port, uint64_t dwconnid)
{
    int current = 0;
    if (ui->tabWidget->count() > 0)
    {
        current = ui->tabWidget->currentIndex();
    }

    WidgetTabContent *content = new WidgetTabContent(protocol, flag, key, ip_4, port, dwconnid, this);
    ui->tabWidget->addTab(content, "-----");
    ui->tabWidget->setCurrentIndex(current);

    QTextCodec *codec = QTextCodec::codecForName(content->getCurrentCodeTypeName().data());
    QString string = codec->toUnicode(QString("连接成功 ---- 地址: %1 ---- 端口: %2").arg(ip_4, QString::number(port)).toStdString().data());
    content->appendData(string, WidgetTabContent::SYSTEM_DATA);

    QString contentKey = QString("%1*%2*%3*%4").arg(key, ip_4, QString::number(port), QString::number(dwconnid));
    mMapContent.insert(contentKey, content);
}

void WidgetTabPages::slot_thread_recv_slave_client_data(const QString &key, const QString &ip_4, uint16_t port, uint64_t dwconnid, const std::string &data)
{
    QString contentKey = QString("%1*%2*%3*%4").arg(key, ip_4, QString::number(port), QString::number(dwconnid));
    WidgetTabContent *content = mMapContent.value(contentKey);
    if (nullptr == content) return;

    QTextCodec *codec = QTextCodec::codecForName(content->getCurrentCodeTypeName().data());
    QString string = codec->toUnicode(QString::fromStdString(data).toStdString().data());

    content->appendData(string);
}

void WidgetTabPages::slot_current_socket_index_change(const QString &socketkey)
{
    if (!mMapContent.contains(socketkey)) return;
    ui->tabWidget->setCurrentWidget(mMapContent.value(socketkey));
}

void WidgetTabPages::slot_slave_client_sent_data_result(const QString &contentKey, bool status, uint32_t length, const QString &error)
{
    if (!mMapContent.contains(contentKey)) return;
    auto widget = mMapContent.value(contentKey);
    if (status)
    {
        widget->applySendResult(length);
    }
    else
    {
        widget->appendError(error);
    }
}

void WidgetTabPages::slot_close_slave_client_result(const QString &socketkey)
{
    WidgetTabContent *content = mMapContent.take(socketkey);
    if (nullptr == content) return;
    int index = ui->tabWidget->indexOf(content);
    if (index < 0) return;
    ui->tabWidget->removeTab(index);
}

void WidgetTabPages::slot_thread_recv_client_data(const QString &key, const QString &ip_4, uint16_t port, uint64_t dwconnid, const std::string &data)
{
    QString contentKey = QString("%1*%2*%3*%4").arg(key, ip_4, QString::number(port), QString::number(dwconnid));
    WidgetTabContent *content = mMapContent.value(contentKey);
    if (nullptr == content)
    {
        LOG_DEBUG(QString("找不到指定窗口 %1").arg(contentKey).toStdString().data());
        return;
    }

    // 这种写法能都防止 string 中含有 0x00 而导致的数据截断
    content->appendData(QString::fromStdString(data));
}

void WidgetTabPages::slot_client_sent_data_result(const QString &contentKey, bool status, uint32_t length, const QString &error)
{
    if (!mMapContent.contains(contentKey)) return;
    auto widget = mMapContent.value(contentKey);
    if (status)
    {
        widget->applySendResult(length);
    }
    else
    {
        widget->appendError(error);
    }
}

void WidgetTabPages::slot_client_closed(const QString &socketkey)
{
    WidgetTabContent *content = mMapContent.take(socketkey);
    if (nullptr == content) return;
    int index = ui->tabWidget->indexOf(content);
    if (index < 0) return;
    ui->tabWidget->removeTab(index);

    // 不主动关闭，删除才关闭
    LOG_DEBUG(QString("删除已关闭的主动连接窗口 %1").arg(socketkey).toStdString().data());
}
