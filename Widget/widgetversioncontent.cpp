#include "widgetversioncontent.h"
#include "ui_widgetversioncontent.h"

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
    QStringList lst = QApplication::applicationVersion().split('.');
    if (lst.count() < 3)
    {
        ui->lbVersion->setText("--");
    }
    else
    {
        ui->lbVersion->setText(QString("00" + lst.at(0)).right(2) + "." +
                               QString("00" + lst.at(1)).right(2) + "." +
                               QString("00" + lst.at(2)).right(2) + "." +
                               QString("00" + lst.at(3)).right(2));
    }
}
