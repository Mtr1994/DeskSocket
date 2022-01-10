#include "dialogserverargs.h"
#include "ui_dialogserverargs.h"

DialogServerArgs::DialogServerArgs(const QString &title, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogServerArgs)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint);
    setWindowTitle(title);

    if (title.contains("广播"))
    {
        ui->tbAddress->setText("255.255.255.255");
        ui->tbAddress->setEnabled(false);
    }
}

DialogServerArgs::~DialogServerArgs()
{
    delete ui;
}

QString DialogServerArgs::getAddress()
{
    return ui->tbAddress->text();
}

QString DialogServerArgs::getPort()
{
    return ui->tbPort->text();
}
void DialogServerArgs::on_btnCancel_clicked()
{
    done(0);
}

void DialogServerArgs::on_btnConfirm_clicked()
{
    done(1);
}
