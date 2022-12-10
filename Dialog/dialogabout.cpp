#include "dialogabout.h"
#include "ui_dialogabout.h"

#include <QLayout>

DialogAbout::DialogAbout(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAbout)
{
    ui->setupUi(this);

    init();
}

DialogAbout::~DialogAbout()
{
    delete ui;
}

void DialogAbout::init()
{
    setWindowTitle("版本信息");
    setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint);

    resize(this->nativeParentWidget()->width() * 0.2, this->nativeParentWidget()->height() * 0.2);

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
