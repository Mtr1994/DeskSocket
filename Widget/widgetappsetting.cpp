#include "widgetappsetting.h"
#include "ui_widgetappsetting.h"
#include "Public/appconstants.h"
#include "Public/appconfig.h"

#include <QListView>
#include <QNetworkInterface>

// test
#include <QDebug>

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

    // 赋值
    QString format = AppConfig::getInstance()->getValue("Setting", "format");
    ui->cbbCodeType->setCurrentText(format);

    connect(ui->btnOk, &QPushButton::clicked, this, &WidgetAppSetting::slot_btn_ok_click);
    connect(ui->btnReset, &QPushButton::clicked, this, &WidgetAppSetting::slot_btn_reset_click);
}

void WidgetAppSetting::slot_btn_ok_click()
{
    // 保存设置
    AppConfig::getInstance()->setValue("Setting", "format", ui->cbbCodeType->currentText());
}

void WidgetAppSetting::slot_btn_reset_click()
{
    QString format = AppConfig::getInstance()->getValue("Setting", "format");
    ui->cbbCodeType->setCurrentText(format);
}
