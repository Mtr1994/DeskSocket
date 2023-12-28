#include "dialognetparameter.h"
#include "ui_dialognetparameter.h"

#include "Public/defines.h"
#include "Public/appsignal.h"
#include "Public/appconfig.h"

#include <QRegExpValidator>

DialogNetParameter::DialogNetParameter(int type, const QString& title, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNetParameter), mNetworkObjectType(type)
{
    ui->setupUi(this);

    init( title);
}

DialogNetParameter::~DialogNetParameter()
{
    delete ui;
}

void DialogNetParameter::init(const QString &title)
{
    ui->tbAddress->setValidator(new QRegExpValidator(QRegExp("((2(5[0-5]|[0-4]\\d))|[0-1]?\\d{1,2})(\\.((2(5[0-5]|[0-4]\\d))|[0-1]?\\d{1,2})){3}")));
    ui->tbPort->setValidator(new QRegExpValidator(QRegExp("([0-9]|[1-9]\\d{1,3}|[1-5]\\d{4}|6[0-4]\\d{4}|65[0-4]\\d{2}|655[0-2]\\d|6553[0-5])")));

    setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint);
    setWindowTitle(title);

    if (mNetworkObjectType == Udp_Client_Cast)
    {
        ui->tbAddress->setText("255.255.255.255");
        ui->tbAddress->setEnabled(false);
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

    emit AppSignal::getInstance()->sgl_prepare_network_object(mNetworkObjectType, address, port);

    AppConfig::getInstance()->setValue("Para", "address", address);
    AppConfig::getInstance()->setValue("Para", "port", QString::number(port));

    done(1);
}
