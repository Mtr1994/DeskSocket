#include "widgetappsetting.h"
#include "ui_widgetappsetting.h"
#include "Public/appconstants.h"
#include "Public/appconfig.h"
#include "Public/appsignal.h"

#include <QListView>
#include <QNetworkInterface>

WidgetAppSetting::WidgetAppSetting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetAppSetting)
{
    ui->setupUi(this);

    init();
}

WidgetAppSetting::~WidgetAppSetting()
{
    delete ui;
}

void WidgetAppSetting::init()
{
    AppConstants::fillComboBox(AppConstants::Item_CodeType, ui->cbbCodeType);
    ui->cbbCodeType->view()->parentWidget()->setWindowFlag(Qt::NoDropShadowWindowHint);

    QList<QNetworkInterface> ifaceList = QNetworkInterface::allInterfaces();
    for(auto &value: ifaceList)
    {
        ui->cbbNetworkCard->addItem(value.humanReadableName());
    }
    ui->cbbNetworkCard->setView(new QListView());
    ui->cbbNetworkCard->view()->parentWidget()->setWindowFlag(Qt::NoDropShadowWindowHint);

    // 日志
    AppConstants::fillComboBox(AppConstants::Item_Log_Type, ui->cbbLogType);
    ui->cbbLogType->view()->parentWidget()->setWindowFlag(Qt::NoDropShadowWindowHint);

    // 赋值
    QString format = AppConfig::getInstance()->getValue("Setting", "format");
    ui->cbbCodeType->setCurrentText(format);

    QString interval = AppConfig::getInstance()->getValue("Setting", "interval");
    ui->tbAutoSendInterval->setText(interval);

    int log = AppConfig::getInstance()->getValue("Setting", "log").toUInt();
    ui->cbbLogType->setCurrentIndex(log);

    // 隐藏
    ui->widgetNetworkSetting->setVisible(false);

    connect(ui->btnOk, &QPushButton::clicked, this, &WidgetAppSetting::slot_btn_ok_click);
    connect(ui->btnReset, &QPushButton::clicked, this, &WidgetAppSetting::slot_btn_reset_click);
}

void WidgetAppSetting::slot_btn_ok_click()
{
    // 保存设置
    AppConfig::getInstance()->setValue("Setting", "format", ui->cbbCodeType->currentText());

    uint32_t interval = ui->tbAutoSendInterval->text().trimmed().toUInt();
    if (interval < 10) interval = 10;
    ui->tbAutoSendInterval->setText(QString::number(interval));

    AppConfig::getInstance()->setValue("Setting", "interval", QString::number(interval));
    AppConfig::getInstance()->setValue("Setting", "log", QString::number(ui->cbbLogType->currentIndex()));

    emit AppSignal::getInstance()->sgl_show_system_toast_message("设置项已保存，部分设置重启后才能生效", 1);
}

void WidgetAppSetting::slot_btn_reset_click()
{
    QString format = AppConfig::getInstance()->getValue("Setting", "format");
    ui->cbbCodeType->setCurrentText(format);

    QString interval = AppConfig::getInstance()->getValue("Setting", "interval");
    ui->tbAutoSendInterval->setText(interval);

    int logType = AppConfig::getInstance()->getValue("Setting", "log").toUInt();
    ui->cbbLogType->setCurrentIndex(logType);
    emit AppSignal::getInstance()->sgl_show_system_toast_message("系统设置重置完成", 1);
}
