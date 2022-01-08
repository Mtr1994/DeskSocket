#include "dialogtcpclientargs.h"
#include "ui_dialogtcpclientargs.h"

DialogTcpClientArgs::DialogTcpClientArgs(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogTcpClientArgs)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint);
    setWindowTitle("客户端参数");
}

DialogTcpClientArgs::~DialogTcpClientArgs()
{
    delete ui;
}

QString DialogTcpClientArgs::getAddress()
{
    return ui->tbAddress->text();
}

QString DialogTcpClientArgs::getPort()
{
    return ui->tbPort->text();
}

void DialogTcpClientArgs::on_btnOk_clicked()
{
    done(1);
}

void DialogTcpClientArgs::on_btnCancel_clicked()
{
    done(0);
}
