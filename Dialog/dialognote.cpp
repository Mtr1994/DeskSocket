#include "dialognote.h"
#include "ui_dialognote.h"

#include <QRectF>

DialogNote::DialogNote(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNote)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_Hover, true);
    this->setMouseTracking(true);
    setStyle();
}

DialogNote::~DialogNote()
{
    delete ui;
    if (nullptr != mMoveingAnimation) delete mMoveingAnimation;
}

void DialogNote::setTitle(const QString &title)
{
    ui->lbTitle->setText(title);
}

void DialogNote::showMessage(const QString &message)
{
    ui->lbContent->setText(message);
    initAnimation();
    mMoveingAnimation->start();
    this->exec();
}

void DialogNote::hideButtonCancel()
{
    ui->btnCancel->setVisible(false);
}

bool DialogNote::event(QEvent *event)
{
    if (event->type() == QEvent::HoverMove)
    {
        bool underMouse =ui->lbClose->geometry().contains(this->mapFromGlobal(QCursor::pos()));
        if (underMouse) ui->lbClose->setStyleSheet("border-image: url(:/resources/image/public/dialog_close_hover.png);");
        else ui->lbClose->setStyleSheet("border-image: url(:/resources/image/public/dialog_close.png);");
    }
    return QDialog::event(event);
}

void DialogNote::mousePressEvent(QMouseEvent *event)
{
    bool underMouse = ui->lbClose->underMouse();
    if (underMouse) return;
    mLeftBtnClk = true;
    mPress = event->pos();
}

void DialogNote::mouseMoveEvent(QMouseEvent *event)
{
    if( mLeftBtnClk ) this->move(event->globalPos() - mPress);
}

void DialogNote::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    bool underMouse =ui->lbClose->geometry().contains(this->mapFromGlobal(QCursor::pos()));
    if (!mLeftBtnClk && underMouse) close();
    mPress = mMove;
    mLeftBtnClk = false;
}

void DialogNote::setStyle()
{
    ui->lbTitle->setStyleSheet("font: 13pt");
}

void DialogNote::initAnimation()
{
    float parentWidth = this->nativeParentWidget()->width();
    float parentHeight = this->nativeParentWidget()->height();
    float parentX = this->nativeParentWidget()->x();
    float parentY = this->nativeParentWidget()->y();
    mMoveingAnimation = new QPropertyAnimation(this, "geometry");
    mMoveingAnimation->setDuration(100);
    mMoveingAnimation->setEasingCurve(QEasingCurve::Linear);
    mMoveingAnimation->setStartValue(QRectF((parentWidth - this->width()) / 2.0 + parentX, (parentHeight - this->height() - 32) / 2.0 + parentY, this->width(), this->height()));
    mMoveingAnimation->setEndValue(QRectF((parentWidth - this->width()) / 2.0 + parentX, (parentHeight - this->height()) / 2.0 + parentY,   this->width(), this->height()));
}

void DialogNote::on_btnConfirm_clicked()
{
    done(1);
}

void DialogNote::on_btnCancel_clicked()
{
    done(0);
}
