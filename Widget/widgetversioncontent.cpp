#include "widgetversioncontent.h"
#include "ui_widgetversioncontent.h"
#include "Public/appconstants.h"

WidgetVersionContent::WidgetVersionContent(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetVersionContent)
{
    ui->setupUi(this);

    init();
}

WidgetVersionContent::~WidgetVersionContent()
{
    delete ui;
}

void WidgetVersionContent::init()
{
    QString version = AppConstants::getValue(AppConstants::Item_Version_Text, "smallVersion");
    ui->lbVersion->setText(version);

    QString qtVersion = AppConstants::getValue(AppConstants::Item_Version_Text, "qtVersion");
    ui->lbQtVersion->setText(QString("Create by %1").arg(qtVersion));
}
