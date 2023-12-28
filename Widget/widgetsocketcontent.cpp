#include "widgetsocketcontent.h"
#include "ui_widgetsocketcontent.h"

#include <QDateTime>
#include <QTextCodec>
#include <QListView>
#include <QMenu>
#include <QAction>

#include "Public/defines.h"
#include "Log/logger.h"
#include "Public/appsignal.h"
#include "Public/appconfig.h"
#include "Network/networkobject.h"

// test
#include <QDebug>
#include <QThread>

WidgetSocketContent::WidgetSocketContent(NetworkObject *netObj, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetSocketContent), mNetworkObject(netObj)
{
    ui->setupUi(this);

    init();
}

WidgetSocketContent::~WidgetSocketContent()
{
    delete ui;
}

void WidgetSocketContent::init()
{
    ui->splitter->setStretchFactor(0, 8);
    ui->splitter->setStretchFactor(1, 2);

    connect(AppSignal::getInstance(), &AppSignal::sgl_update_network_object, this, &WidgetSocketContent::slot_update_network_object);
    connect(AppSignal::getInstance(), &AppSignal::sgl_recv_network_data, this, &WidgetSocketContent::slot_recv_network_data);

    connect(ui->tbDatas, &QTextEdit::customContextMenuRequested, this, &WidgetSocketContent::slot_tb_logs_costom_menu_request);
    connect(ui->tbSendDatas, &QTextEdit::customContextMenuRequested, this, &WidgetSocketContent::slot_tb_sender_custom_menu_request);

    connect(ui->btnSend, &QPushButton::clicked, this, &WidgetSocketContent::slot_btn_send_click);

    appendMessage("正在初始化套接字 ... ");
}

void WidgetSocketContent::appendMessage(const QString &msg)
{
    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz");
    ui->tbDatas->append("<p style=\"color:#999999; font-size: 8pt;\">" + time + "</p>" + "<p color='#fd5401'> &nbsp;&nbsp;&nbsp;&nbsp;" + msg + "</p>");
}

void WidgetSocketContent::clearStatisticsNumber()
{
    mTotalSendBytes = 0;
    ui->lbSendBytes->setText(QString("发送字节数：%1").arg(QString::number(mTotalSendBytes)));

    mTotalRecvBytes = 0;
    ui->lbRecvBytes->setText(QString("接收字节数：%1").arg(QString::number(mTotalRecvBytes)));
}

void WidgetSocketContent::setDwConnID(int32_t id)
{
    mDwConnID = id;
    slot_update_network_object(mNetworkObject->getObjectDetail(mDwConnID).token);
}

/**
 * @brief WidgetSocketContent::on_btnSend_clicked
 * 发送数据接口
 */

void WidgetSocketContent::slot_btn_send_click()
{
    if (nullptr == mNetworkObject) return;
    if (mNetworkObject->getObjectDetail().status != 1) return;

    if (ui->tbSendDatas->toPlainText().isEmpty()) return;
    QByteArray package;
    if (ui->menuHexSend->isChecked())
    {
        package = QByteArray::fromHex(ui->tbSendDatas->toPlainText().trimmed().toStdString().data());
    }
    else
    {
        package = QByteArray::fromStdString(ui->tbSendDatas->toPlainText().toStdString().data());

        QString codeName = AppConfig::getInstance()->getValue("Setting", "format");
        if (codeName.isEmpty()) codeName = "UTF-8";
        QTextCodec *codec = QTextCodec::codecForName(codeName.toStdString().data());
        package = codec->fromUnicode(QString(package));
    }

    mTotalSendBytes += package.length();
    updateStatistics();

    qDebug() << "WidgetSocketContent::slot_btn_send_click" << QThread::currentThreadId();
    mNetworkObject->send(package.data(), (uint32_t)package.length(), mDwConnID);
}

void WidgetSocketContent::slot_tb_logs_costom_menu_request(const QPoint &pos)
{
    Q_UNUSED(pos);
    QMenu menu(this);
    menu.setWindowFlags(Qt::NoDropShadowWindowHint | menu.windowFlags() | Qt::FramelessWindowHint);
    menu.setAttribute(Qt::WA_TranslucentBackground);
    QAction actionCopy("复制");
    // 选中才能复制
    connect(&actionCopy, &QAction::triggered, this, [this]() { ui->tbDatas->copy(); });
    QAction actionClear("全部清理");
    connect(&actionClear, &QAction::triggered, this, [this]() { ui->tbDatas->clear(); });

    QAction actionClearStatistics("重置统计数据");
    connect(&actionClearStatistics, &QAction::triggered, this, [this]() { this->clearStatisticsNumber(); });

    menu.addAction(&actionCopy);
    menu.addAction(&actionClear);
    menu.addAction(&actionClearStatistics);

    menu.exec(QCursor::pos());
}

void WidgetSocketContent::slot_tb_sender_custom_menu_request(const QPoint &pos)
{
    Q_UNUSED(pos);
    QMenu menu(this);
    menu.setWindowFlags(Qt::NoDropShadowWindowHint | menu.windowFlags() | Qt::FramelessWindowHint);
    menu.setAttribute(Qt::WA_TranslucentBackground);
    QAction actionCopy("复制");
    connect(&actionCopy, &QAction::triggered, this, [this]() { ui->tbSendDatas->copy(); });
    QAction actionPaste("粘贴");
    connect(&actionPaste, &QAction::triggered, this, [this]() { ui->tbSendDatas->paste(); });
    QAction actionCut("剪切");
    connect(&actionCut, &QAction::triggered, this, [this]() { ui->tbSendDatas->cut(); });
    QAction actionClear("全部清理");
    connect(&actionClear, &QAction::triggered, this, [this]() { ui->tbSendDatas->clear(); });

    menu.addAction(&actionCopy);
    menu.addAction(&actionPaste);
    menu.addAction(&actionClear);
    menu.addAction(&actionCut);

    menu.exec(QCursor::pos());
}

void WidgetSocketContent::slot_update_network_object(const QString &token)
{
    qDebug() << "WidgetSocketContent::slot_update_network_object";
    if (nullptr == mNetworkObject) return;

    NetworkObjectDetail detail = mNetworkObject->getObjectDetail(mDwConnID);
    if (detail.token != token) return;
    if (detail.status == 0)
    {
        QString msg = QString("%1 - %2  正在连接").arg(detail.peerAddress, QString::number(detail.peerPort));
        appendMessage(msg);
    }
    else if (detail.status == 1)
    {
        QString msg = QString("%1 - %2  连接成功").arg(detail.peerAddress, QString::number(detail.peerPort));
        appendMessage(msg);
    }
    else if (detail.status == 2)
    {
        QString msg = QString("%1 - %2  正在关闭").arg(detail.peerAddress, QString::number(detail.peerPort));
        appendMessage(msg);
    }
    else if (detail.status == 3)
    {
        QString msg = QString("%1 - %2  连接断开").arg(detail.peerAddress, QString::number(detail.peerPort));
        appendMessage(msg);
    }
}

void WidgetSocketContent::slot_recv_network_data(const QString& token, const std::string &data, uint16_t length)
{
    qDebug() << "slot_recv_network_data1 " << length << token;
    if (nullptr == mNetworkObject) return;
    NetworkObjectDetail detail = mNetworkObject->getObjectDetail(mDwConnID);
    qDebug() << "slot_recv_network_data2 " << detail.token;
    if (detail.token != token) return;

    mTotalRecvBytes += length;
    updateStatistics();

    std::string recvData = data;
    if (ui->menuHexRecv->isChecked()) recvData = QByteArray::fromStdString(data).toHex().toStdString();
    appendMessage(recvData.data());
}

void WidgetSocketContent::updateStatistics()
{
    ui->lbSendBytes->setText(QString("发送字节数：%1").arg(QString::number(mTotalSendBytes)));
    ui->lbRecvBytes->setText(QString("接收字节数：%1").arg(QString::number(mTotalRecvBytes)));
}
