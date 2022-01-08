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
}
