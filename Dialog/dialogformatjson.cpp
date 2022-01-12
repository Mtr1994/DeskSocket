#include "dialogformatjson.h"
#include "ui_dialogformatjson.h"

#include <QJsonDocument>
#include <QJsonParseError>
#include <QClipboard>

DialogFormatJson::DialogFormatJson(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogFormatJson)
{
    ui->setupUi(this);

    init();
}

DialogFormatJson::~DialogFormatJson()
{
    delete ui;
}

void DialogFormatJson::init()
{
    setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint);
    setWindowTitle("JSON 字符串格式化工具");

    connect(ui->btnFormatJson, &QPushButton::clicked, this, &DialogFormatJson::slot_format_click);
    connect(ui->btnCopyResult, &QPushButton::clicked, this, &DialogFormatJson::slot_copy_click);

    connect(ui->tbValue, &QTextEdit::textChanged, this, &DialogFormatJson::slot_text_value_change);
}

void DialogFormatJson::slot_format_click()
{
    QString value = ui->tbValue->toPlainText();
    if (value.length() == 0)
    {
        ui->lbMessage->setText("<p style=\"color:#1890ff; font-size:50%\">数据体为空</p>");
        return;
    }

    QJsonParseError parseError;
    QJsonDocument document = QJsonDocument::fromJson(value.toStdString().data(), &parseError);
    if (parseError.error != QJsonParseError::NoError)
    {
        ui->lbMessage->setText("<p style=\"color:#f5222d; font-size:50%\">转换失败：" + parseError.errorString() + "</p>");
        return;
    }

    uint8_t flag = mFormatCount % 2;
    if (!flag)
    {
        ui->tbValue->setText(document.toJson(QJsonDocument::Indented));
        ui->btnFormatJson->setText("格式化（缩进模式）");
    }
    else
    {
        ui->tbValue->setText(document.toJson(QJsonDocument::Compact));
        ui->btnFormatJson->setText("格式化（紧凑模式）");
    }
    mFormatCount++;

    ui->lbMessage->setText("<p style=\"color:#52c41a; font-size:50%\">格式化成功</p>");
}

void DialogFormatJson::slot_copy_click()
{
    QString value = ui->tbValue->toPlainText();
    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(value);

    ui->lbMessage->setText("<p style=\"color:#1890ff; font-size:50%\">拷贝完成</p>");
}

void DialogFormatJson::slot_text_value_change()
{
    ui->lbMessage->clear();
}
