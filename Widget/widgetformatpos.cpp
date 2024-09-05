#include "widgetformatpos.h"
#include "ui_widgetformatpos.h"
#include "Public/appsignal.h"
#include "Log/logger.h"

#include <QRegularExpression>
#include <QRegExp>
#include <QRegularExpressionValidator>
#include <QClipboard>

WidgetFormatPos::WidgetFormatPos(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WidgetFormatPos)
{
    ui->setupUi(this);

    init();
}

WidgetFormatPos::~WidgetFormatPos()
{
    delete ui;
}

void WidgetFormatPos::init()
{
    connect(ui->btnFormat2dms, &QPushButton::clicked, this, &WidgetFormatPos::slot_btn_format_to_dms_click);
    connect(ui->btnformat2pos, &QPushButton::clicked, this, &WidgetFormatPos::slot_btn_format_to_pos_click);
    connect(ui->btnCopyDms, &QPushButton::clicked, this, &WidgetFormatPos::slot_btn_copy_dms_click);
    connect(ui->btnCopyPos, &QPushButton::clicked, this, &WidgetFormatPos::slot_btn_copy_pos_click);

    //
    QRegularExpression regExp("^(-?\\d+(\\.\\d+)?),\\s*(-?\\d+(\\.\\d+)?)$");
    ui->tbLongitude->setValidator(new QRegularExpressionValidator(regExp));
    ui->tbLatitude->setValidator(new QRegularExpressionValidator(regExp));
}

void WidgetFormatPos::slot_btn_format_to_dms_click()
{
    ui->lbFormatPosResultA->setText("等待转换");
    double longitude = ui->tbLongitude->text().trimmed().toDouble();
    double latitude = ui->tbLatitude->text().trimmed().toDouble();

    bool valid = true;
    if (longitude > 180 || longitude< -180) valid = false;
    if (latitude > 90 || latitude< -90) valid = false;
    if (!valid)
    {
        emit AppSignal::getInstance()->sgl_show_system_toast_message("请输入有效的纬度和经度", 2);
        return;
    }

    bool isNagtiveLon = longitude < 0;
    bool isNagtiveLat = latitude < 0;

    longitude = abs(longitude);
    latitude = abs(latitude);

    double d, m, s;
    d = longitude;
    m = (longitude - floor(d)) * 60;
    s = (longitude - floor(d) - floor(m) / 60) * 3600;
    QString lonStr = QString("%1%2°%3′%4″").arg(isNagtiveLon ? "-" : "", QString::number(floor(d)), QString::number(floor(m)), QString::number(s, 'f', 4));

    d = latitude;
    m = (latitude - floor(d)) * 60;
    s = (latitude - floor(d) - floor(m) / 60) * 3600;
    QString latStr = QString("%1%2°%3′%4″").arg(isNagtiveLat ? "-" : "", QString::number(floor(d)), QString::number(floor(m)), QString::number(s, 'f', 4));

    ui->lbFormatPosResultA->setText(QString("经度: %1   纬度: %2").arg(lonStr, latStr));
}

void WidgetFormatPos::slot_btn_copy_dms_click()
{
    QString content = ui->lbFormatPosResultB->text().trimmed();
    QApplication::clipboard()->setText(content);
    emit AppSignal::getInstance()->sgl_show_system_toast_message(QString("已复制到剪贴板: %1").arg(content), 1);
}

void WidgetFormatPos::slot_btn_format_to_pos_click()
{
    ui->lbFormatPosResultB->setText("等待转换");
    QString dms = ui->tbdms->text().trimmed();
    QStringList list;
    list = dms.split("°");
    if (list.size() != 2)
    {
        emit AppSignal::getInstance()->sgl_show_system_toast_message("请输入有效的度分秒数据 Ⅰ", 2);
        return;
    }

    double d, m, s;
    d = list.at(0).toInt();

    list = list.at(1).contains("′") ? list.at(1).split("′") : list.at(1).split("'");
    if (list.size() != 2)
    {
        emit AppSignal::getInstance()->sgl_show_system_toast_message("请输入有效的度分秒数据 Ⅱ", 2);
        return;
    }
    m = list.at(0).toInt();

    list = list.at(1).contains("″") ? list.at(1).split("″") : list.at(1).split("\"");
    if (list.size() != 2)
    {
        emit AppSignal::getInstance()->sgl_show_system_toast_message("请输入有效的度分秒数据 Ⅲ", 2);
        return;
    }
    s = list.at(0).toDouble();

    QString suffix;
    bool isNagtivePos = false;
    if (list.size() >= 2)
    {
        suffix = list.at(1);
        isNagtivePos = suffix.contains("S", Qt::CaseInsensitive) || list.at(1).contains("W", Qt::CaseInsensitive);
    }

    if (d < 0) isNagtivePos = !isNagtivePos;
    double pos = abs(d) + m / 60 + s / 3600;
    if (pos > 180)
    {
        emit AppSignal::getInstance()->sgl_show_system_toast_message("请输入有效的度分秒数据 Ⅳ", 2);
        return;
    }
    if ((pos > 90) && ((suffix.contains("S",  Qt::CaseInsensitive)) || (suffix.contains("N",  Qt::CaseInsensitive))))
    {
        emit AppSignal::getInstance()->sgl_show_system_toast_message("请输入有效的度分秒数据 Ⅴ", 2);
        return;
    }
    QString posStr = QString::number(pos, 'f', 6);
    ui->lbFormatPosResultB->setText(QString("经度/纬度：%1%2").arg(isNagtivePos ? "-" : "", posStr));
}

void WidgetFormatPos::slot_btn_copy_pos_click()
{
    QString content = ui->lbFormatPosResultA->text().trimmed();
    QApplication::clipboard()->setText(content);
    emit AppSignal::getInstance()->sgl_show_system_toast_message(QString("已复制到剪贴板: %1").arg(content), 1);
}
