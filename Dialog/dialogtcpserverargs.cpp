#include "dialogtcpserverargs.h"
#include "ui_dialogtcpserverargs.h"

DialogTcpServerArgs::DialogTcpServerArgs(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogTcpServerArgs)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint);
    setWindowTitle("服务器参数");
}

DialogTcpServerArgs::~DialogTcpServerArgs()
{
    delete ui;
}

QString DialogTcpServerArgs::getAddress()
{
    return ui->tbAddress->text();
}

QString DialogTcpServerArgs::getPort()
{
    return ui->tbPort->text();
}
void DialogTcpServerArgs::on_btnCancel_clicked()
{
    done(0);
}

void DialogTcpServerArgs::on_btnConfirm_clicked()
{


    done(1);
}
