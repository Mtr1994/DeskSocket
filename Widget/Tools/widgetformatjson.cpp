#include "widgetformatjson.h"
#include "ui_widgetformatjson.h"
#include "Public/appsignal.h"

#include <QJsonDocument>
#include <QJsonParseError>
#include <QClipboard>
#include <QMenu>
#include <QMimeData>

WidgetFormatJson::WidgetFormatJson(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetFormatJson)
{
    ui->setupUi(this);

    init();
}

WidgetFormatJson::~WidgetFormatJson()
{
    delete ui;
}

void WidgetFormatJson::init()
{
    connect(ui->btnFormatJsonCompact, &QPushButton::clicked, this, &WidgetFormatJson::slot_btn_format_click);
    connect(ui->btnFormatJsonIndented, &QPushButton::clicked, this, &WidgetFormatJson::slot_btn_format_click);
    connect(ui->tbValue, &QTextEdit::customContextMenuRequested, this, &WidgetFormatJson::slot_json_custom_menu_request);
}

void WidgetFormatJson::slot_btn_format_click()
{
    QString value = ui->tbValue->toPlainText();
    if (value.length() == 0)
    {
        emit AppSignal::getInstance()->sgl_show_system_toast_message("数据为空，无法格式化", 3);
        return;
    }

    QJsonParseError parseError;
    QJsonDocument document = QJsonDocument::fromJson(value.toStdString().data(), &parseError);
    if (parseError.error != QJsonParseError::NoError)
    {
        emit AppSignal::getInstance()->sgl_show_system_toast_message(QString("转换失败 %1").arg(parseError.errorString()), 2);
        return;
    }

    QJsonDocument::JsonFormat format = (sender()->objectName().contains("Compact")) ? QJsonDocument::Compact : QJsonDocument::Indented;
    ui->tbValue->setText(document.toJson(format));

    emit AppSignal::getInstance()->sgl_show_system_toast_message("格式化成功", 1);
}

void WidgetFormatJson::slot_json_custom_menu_request(const QPoint &pos)
{
    Q_UNUSED(pos);
    QMenu menu(this);
    menu.setWindowFlags(Qt::NoDropShadowWindowHint | menu.windowFlags() | Qt::FramelessWindowHint);
    menu.setAttribute(Qt::WA_TranslucentBackground);
    QAction actionCopy("复制内容");
    connect(&actionCopy, &QAction::triggered, this, [this]()
    {
        QString selectText = ui->tbValue->textCursor().selectedText();
        if (selectText.isEmpty())
        {
            ui->tbValue->copy();
        }
        else
        {
            QClipboard* pClipboard = QApplication::clipboard();
            pClipboard->setText(ui->tbValue->toPlainText());
        }
        emit AppSignal::getInstance()->sgl_show_system_toast_message("内容已复制", 1);
    });

    QAction actionPaste("粘贴内容");
    connect(&actionPaste, &QAction::triggered, this, [this]() { ui->tbValue->paste(); });

    QAction actionClear("清空内容");
    connect(&actionClear, &QAction::triggered, this, [this]() { ui->tbValue->clear(); });

    menu.addAction(&actionCopy);
    menu.addAction(&actionPaste);
    menu.addAction(&actionClear);

    menu.exec(QCursor::pos());
}
