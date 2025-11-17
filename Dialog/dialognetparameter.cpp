#include "dialognetparameter.h"
#include "ui_dialognetparameter.h"

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <QRegExpValidator>
#else
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#endif

#include "Public/defines.h"
#include "Public/appsignal.h"
#include "Public/appconfig.h"

DialogNetParameter::DialogNetParameter(int type, const QString& title, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNetParameter), mNetworkObjectType(type)
{
    ui->setupUi(this);

    init(title);
}

DialogNetParameter::DialogNetParameter(const QString& address, const QString& port, const QString& token, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNetParameter)
{
    ui->setupUi(this);
    mIsEditStatus = true;
    mNetObjectToken = token;

    ui->tbAddress->setText(address.trimmed());
    ui->tbPort->setText(port.trimmed());

    init("编辑参数");
}

DialogNetParameter::~DialogNetParameter()
{
    delete ui;
}

void DialogNetParameter::init(const QString &title)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    ui->tbAddress->setValidator(new QRegExpValidator(QRegExp("((2(5[0-5]|[0-4]\\d))|[0-1]?\\d{1,2})(\\.((2(5[0-5]|[0-4]\\d))|[0-1]?\\d{1,2})){3}")));
    ui->tbPort->setValidator(new QRegExpValidator(QRegExp("([0-9]|[1-9]\\d{1,3}|[1-5]\\d{4}|6[0-4]\\d{4}|65[0-4]\\d{2}|655[0-2]\\d|6553[0-5])")));
#else
    ui->tbAddress->setValidator(new QRegularExpressionValidator(QRegularExpression("((2(5[0-5]|[0-4]\\d))|[0-1]?\\d{1,2})(\\.((2(5[0-5]|[0-4]\\d))|[0-1]?\\d{1,2})){3}")));
    ui->tbPort->setValidator(new QRegularExpressionValidator(QRegularExpression("([0-9]|[1-9]\\d{1,3}|[1-5]\\d{4}|6[0-4]\\d{4}|65[0-4]\\d{2}|655[0-2]\\d|6553[0-5])")));
#endif
    setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint);
    setWindowTitle(title);

    if (mIsEditStatus)
    {
        // add logoc here
    }
    else
    {
        ui->tbAddress->setText(AppConfig::getInstance()->getValue("Para", "address"));
        ui->tbPort->setText(AppConfig::getInstance()->getValue("Para", "port"));
    }
    connect(ui->btnCancel, &QPushButton::clicked, this, &DialogNetParameter::slot_btn_cancel_clicked);
    connect(ui->btnConfirm, &QPushButton::clicked, this, &DialogNetParameter::slot_btn_confirm_clicked);
}

void DialogNetParameter::slot_btn_cancel_clicked()
{
    done(0);
}

void DialogNetParameter::slot_btn_confirm_clicked()
{
    QString address = ui->tbAddress->text();
    uint16_t port = ui->tbPort->text().trimmed().toUInt();

    if (mIsEditStatus)
    {
        emit AppSignal::getInstance()->sgl_recreate_network_object(mNetObjectToken, address, port);
    }
    else
    {
        emit AppSignal::getInstance()->sgl_create_network_object(mNetworkObjectType, address, port);
    }

    AppConfig::getInstance()->setValue("Para", "address", address);
    AppConfig::getInstance()->setValue("Para", "port", QString::number(port));

    done(1);
}
