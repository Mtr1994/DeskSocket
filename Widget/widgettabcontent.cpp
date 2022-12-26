#include "widgettabcontent.h"
#include "ui_widgettabcontent.h"

#include <QDateTime>
#include <QTextCodec>
#include <QListView>
#include <QMenu>
#include <QAction>

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

    connect(ui->tbDatas, &QTextEdit::customContextMenuRequested, this, &WidgetTabContent::slot_tb_logs_costom_menu_request);

    SoftConstants::fillComboBox(SoftConstants::Item_CodeType, ui->cbbCodeType);
    ui->cbbCodeType->view()->parentWidget()->setWindowFlag(Qt::NoDropShadowWindowHint);

    connect(ui->btnSend, &QPushButton::clicked, this, &WidgetTabContent::slot_btn_send_click);
}

void WidgetTabContent::appendError(const QString &data)
{
    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz");
    ui->tbDatas->append("<p style=\"color:#999999; font-size: 8pt;\">" + time + ":错误</p>" + "<p color='#fd5401'> &nbsp;&nbsp;&nbsp;&nbsp;" + data + "</p>");
}

void WidgetTabContent::appendData(const QString &data, int type)
{
    // 不回显发送的内容
    if (type == SEND_DATA) return;

    if (type == RECV_DATA)
    {
        mTotalRecvBytes += QByteArray::fromStdString(data.toStdString()).length();
        ui->lbRecvBytes->setText(QString("接收：%1").arg(QString::number(mTotalRecvBytes)));
    }

    QString tip = (type == RECV_DATA) ? "接收" : (type == SEND_DATA) ? "发送" : "系统";
    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz");

    QString text = data;
    if (ui->menuHexRecv->isChecked() && (type == RECV_DATA))
    {
        text = QByteArray::fromStdString(text.toStdString()).toHex().toStdString().data();
    }
    else
    {
        QTextCodec *codec = QTextCodec::codecForName(ui->cbbCodeType->currentText().toStdString().data());
        text = codec->toUnicode(data.toStdString().data());
    }

    ui->tbDatas->append("<p style=\"color:#999999; font-size: 8pt;\">" + time + ": (" + tip + ")</p>" + "<p color='#666666'> &nbsp;&nbsp;&nbsp;&nbsp;" + text + "</p>");
}

void WidgetTabContent::applySendResult(int length)
{
    if (length == 0) return appendError("发送失败");

    mTotalSendBytes += length;
    ui->lbSendBytes->setText(QString("发送：%1").arg(QString::number(mTotalSendBytes)));

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

    if (ui->menuHexSend->isChecked())
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

void WidgetTabContent::slot_tb_logs_costom_menu_request(const QPoint &pos)
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

    menu.addAction(&actionCopy);
    menu.addAction(&actionClear);

    menu.exec(QCursor::pos());
}
