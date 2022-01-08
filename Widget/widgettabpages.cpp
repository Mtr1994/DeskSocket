#include "widgettabpages.h"
#include "ui_widgettabpages.h"

#include <QTabBar>
#include <QTextCodec>

#include "Widget/widgettabcontent.h"
#include "Public/appsignal.h"

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
    connect(AppSignal::getInstance(), &AppSignal::sgl_add_tcp_tab_page, this, &WidgetTabPages::slot_add_tcp_tab_page);
    connect(AppSignal::getInstance(), &AppSignal::sgl_thread_recv_slave_tcp_client_data, this, &WidgetTabPages::slot_thread_recv_slave_tcp_client_data, Qt::QueuedConnection);
    connect(AppSignal::getInstance(), &AppSignal::sgl_current_socket_index_change, this, &WidgetTabPages::slot_current_socket_index_change);
    connect(AppSignal::getInstance(), &AppSignal::sgl_slave_tcp_client_sent_data_result, this, &WidgetTabPages::slot_slave_tcp_client_sent_data_result);
    connect(AppSignal::getInstance(), &AppSignal::sgl_close_slave_tcp_client_result, this, &WidgetTabPages::slot_close_slave_tcp_client_result);
    connect(AppSignal::getInstance(), &AppSignal::sgl_thread_recv_tcp_client_data, this, &WidgetTabPages::slot_thread_recv_tcp_client_data);
    connect(AppSignal::getInstance(), &AppSignal::sgl_tcp_client_sent_data_result, this, &WidgetTabPages::slot_tcp_client_sent_data_result);
    connect(AppSignal::getInstance(), &AppSignal::sgl_tcp_client_closed, this, &WidgetTabPages::sgl_tcp_client_closed);
}

int WidgetTabPages::getTabCount()
{
    return ui->tabWidget->count();
}

void WidgetTabPages::removeTab(int socketptr)
{
//    WidgetTabContent *content = mMapContent.take(socketptr);
//    if (nullptr == content) return;
//    int index = ui->tabWidget->indexOf(content);
//    if (index < 0) return;
//    ui->tabWidget->removeTab(index);
}

void WidgetTabPages::appentData(int socketptr, const QByteArray &data)
{
//    WidgetTabContent *content = mMapContent.value(socketptr);
//    if (nullptr == content) return;

//    QTextCodec *codec = QTextCodec::codecForName(content->getCurrentCodeTypeName().data());
//    QString string = codec->toUnicode(data);

//    content->appendData(string);
}

void WidgetTabPages::slot_add_tcp_tab_page(const QString &flag, const QString &key, const QString &ip_4, uint16_t port, uint64_t dwconnid)
{
    int current = 0;
    if (ui->tabWidget->count() > 0)
    {
        current = ui->tabWidget->currentIndex();
    }

    WidgetTabContent *content = new WidgetTabContent(flag, key, ip_4, port, dwconnid, this);
    ui->tabWidget->addTab(content, "-----");
    ui->tabWidget->setCurrentIndex(current);

    QTextCodec *codec = QTextCodec::codecForName(content->getCurrentCodeTypeName().data());
    QString string = codec->toUnicode(QString("连接成功 ---- 地址: %1 ---- 端口: %2").arg(ip_4, QString::number(port)).toStdString().data());
    content->appendData(string, WidgetTabContent::SYSTEM_DATA);

    connect(content, &WidgetTabContent::sgl_client_operation, this, &WidgetTabPages::sgl_client_operation);

    QString contentKey = QString("%1*%2*%3*%4").arg(key, ip_4, QString::number(port), QString::number(dwconnid));
    mMapContent.insert(contentKey, content);
}

void WidgetTabPages::slot_thread_recv_slave_tcp_client_data(const QString &key, const QString &ip_4, uint16_t port, uint64_t dwconnid, const QByteArray &data)
{
    QString contentKey = QString("%1*%2*%3*%4").arg(key, ip_4, QString::number(port), QString::number(dwconnid));
    WidgetTabContent *content = mMapContent.value(contentKey);
    if (nullptr == content) return;

    QTextCodec *codec = QTextCodec::codecForName(content->getCurrentCodeTypeName().data());
    QString string = codec->toUnicode(data);

    content->appendData(string);
}

void WidgetTabPages::slot_current_socket_index_change(const QString &socketkey)
{
    if (!mMapContent.contains(socketkey)) return;
    ui->tabWidget->setCurrentWidget(mMapContent.value(socketkey));
}

void WidgetTabPages::slot_slave_tcp_client_sent_data_result(const QString &contentKey, bool status, uint32_t length, const QString &error)
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

void WidgetTabPages::slot_close_slave_tcp_client_result(const QString &socketkey)
{
    WidgetTabContent *content = mMapContent.take(socketkey);
    if (nullptr == content) return;
    int index = ui->tabWidget->indexOf(content);
    if (index < 0) return;
    ui->tabWidget->removeTab(index);
}

void WidgetTabPages::slot_thread_recv_tcp_client_data(const QString &key, const QString &ip_4, uint16_t port, uint64_t dwconnid, const QByteArray &data)
{
    QString contentKey = QString("%1*%2*%3*%4").arg(key, ip_4, QString::number(port), QString::number(dwconnid));
    qDebug() << "recv key " << contentKey;
    WidgetTabContent *content = mMapContent.value(contentKey);
    if (nullptr == content)
    {
        qDebug() << "找不到指定的窗口";
        return;
    }

    QTextCodec *codec = QTextCodec::codecForName(content->getCurrentCodeTypeName().data());
    QString string = codec->toUnicode(data);

    content->appendData(string);
}

void WidgetTabPages::slot_tcp_client_sent_data_result(const QString &contentKey, bool status, uint32_t length, const QString &error)
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

void WidgetTabPages::sgl_tcp_client_closed(const QString &socketkey)
{
    WidgetTabContent *content = mMapContent.take(socketkey);
    if (nullptr == content) return;
    int index = ui->tabWidget->indexOf(content);
    if (index < 0) return;
    ui->tabWidget->removeTab(index);
}
