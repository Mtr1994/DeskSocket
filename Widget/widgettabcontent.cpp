#include "widgettabcontent.h"
#include "ui_widgettabcontent.h"

#include <QDateTime>
#include <QTextCodec>

#include "Public/defines.h"
#include "Log/logger.h"
#include "Public/softconstants.h"
#include "Public/appsignal.h"

WidgetTabContent::WidgetTabContent(uint16_t protocol, const QString &flag, const QString &key, const QString &ip_4, uint16_t port, uint64_t dwconnid, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetTabContent), mProtocol(protocol), mContentFlag(flag), mServerKey(key), mClientAddress(ip_4),mClientPort(port),mClientID(dwconnid)
{
    ui->setupUi(this);

    init();
}

WidgetTabContent::~WidgetTabContent()
{
    delete ui;
}

void WidgetTabContent::init()
{
    ui->splitter->setStretchFactor(0, 8);
    ui->splitter->setStretchFactor(1, 2);

    SoftConstants::fillComboBox(SoftConstants::Item_CodeType, ui->cbbCodeType);

    connect(ui->btnSend, &QPushButton::clicked, this, &WidgetTabContent::slot_btn_send_click);
}

void WidgetTabContent::appendError(const QString &data)
{
    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz");
    ui->tbDatas->append("<p style=\"font-family:Perpetua; color:#999999; font-size:50%\">" + time + ":错误</p>" + "<p color='#fd5401'> &nbsp;&nbsp;&nbsp;&nbsp;" + data + "</p>");
}

void WidgetTabContent::appendData(const QString &data, int type)
{
    QString tip = (type == RECV_DATA) ? "接收" : (type == SEND_DATA) ? "发送" : "系统";
    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz");

    QString text = data;
    if (ui->menuHex->isChecked())
    {
        text = QByteArray::fromStdString(text.toStdString()).toHex().toStdString().data();
    }
    else
    {
        QTextCodec *codec = QTextCodec::codecForName(ui->cbbCodeType->currentText().toStdString().data());
        text = codec->toUnicode(data.toStdString().data());
    }

    ui->tbDatas->append("<p style=\"font-family:Perpetua; color:#999999; font-size:50%\">" + time + ": (" + tip + ")</p>" + "<p color='#666666'> &nbsp;&nbsp;&nbsp;&nbsp;" + text + "</p>");
}

void WidgetTabContent::applySendResult(int length)
{
    if (length == 0) return appendError("发送失败");

    QByteArray array = QByteArray::fromStdString(ui->tbSendDatas->toPlainText().toStdString());
    appendData(QString::fromStdString(array.toStdString()), SEND_DATA);
    // ui->tbSendDatas->clear();
    ui->tbSendDatas->clearFocus();
}

std::string WidgetTabContent::getCurrentCodeTypeName()
{
    return ui->cbbCodeType->currentText().toStdString().data();
}

/**
 * @brief WidgetTabContent::on_btnSend_clicked
 * 发送数据接口
 */

void WidgetTabContent::slot_btn_send_click()
{
    if (ui->tbSendDatas->toPlainText().isEmpty()) return;
    std::string package;

    if (ui->menuHex->isChecked())
    {
        package = QByteArray::fromHex(ui->tbSendDatas->toPlainText().trimmed().toStdString().data()).toStdString();
    }
    else
    {
        package = QByteArray::fromStdString(ui->tbSendDatas->toPlainText().toStdString().data()).toStdString();
    }

    QTextCodec *codec = QTextCodec::codecForName(ui->cbbCodeType->currentText().toStdString().data());
    package = codec->fromUnicode(QString::fromStdString(package)).toStdString();

    QString contentKey = QString("%1*%2*%3*%4").arg(mServerKey, mClientAddress, QString::number(mClientPort), QString::number(mClientID));
    if (mContentFlag == "SLAVECLIENT")
    {
        emit AppSignal::getInstance()->sgl_slave_client_sent_data(mServerKey, contentKey, mClientID, package);
    }
    else if (mContentFlag == "CLIENT")
    {
        emit AppSignal::getInstance()->sgl_client_sent_data(mServerKey, contentKey, mClientID, package);
    }
}
