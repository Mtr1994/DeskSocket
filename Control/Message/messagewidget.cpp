#include "messagewidget.h"

#include <QPainter>
#include <QRect>
#include <QFontMetrics>
#include <QTimer>
#include <QPropertyAnimation>
#include <QPoint>
#include <QtMath>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QGraphicsDropShadowEffect>

// test
#include <QDebug>

QList<int> MessageWidget::mListSerialNumber = {};

MessageWidget::MessageWidget(int type, int position, QWidget *parent)
    : QWidget{parent}, mParentWidget(parent), mMessageType(type), mMessagePosition(position)
{
    init();
}

void MessageWidget::showMessage()
{
    if (nullptr == mParentWidget)
    {
        delete this;
        return;
    }

    int number = mListSerialNumber.size();
    if (number >= mMaxMessageNumber) return;

    for (int i = 1; i <= mMaxMessageNumber; i++)
    {
        if (!mListSerialNumber.contains(i))
        {
            mSerialNumber = i;
            mListSerialNumber.append(i);
            break;
        }
    }

    if (mSerialNumber < 0)
    {
        delete mAnimation;
        delete this;
        return;
    }

    // 定义动画
    animateDesign();

    mAnimation->start();

    setVisible(true);
}

void MessageWidget::showMessage(const QString &message)
{
    mMessage = message;
    if (nullptr != mLabelMessage) mLabelMessage->setText(mMessage);
    showMessage();
}

void MessageWidget::init()
{
    QFont font("Microsoft YaHei", 9);
    QFontMetrics metrics(font);
    QRect fontRect = metrics.boundingRect("H");
    mContentMargin = qCeil(fontRect.height() * 0.24);

    QWidget *widgetBase = new QWidget(this);
    widgetBase->setStyleSheet("QWidget{background-color: #fefefe;border-radius: 0.3em;}");

    QLabel *lbImage = new QLabel(widgetBase);
    lbImage->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));
    lbImage->setMinimumWidth(lbImage->height() * 0.64);
    QString url = (mMessageType == M_Success) ? ":/Resource/image/success.svg" : (mMessageType == M_Error) ? ":/Resource/image/error.svg" : ":/Resource/image/info.svg";
    lbImage->setStyleSheet(QString("QLabel{image: url(%1);}").arg(url));

    mLabelMessage = new QLabel(widgetBase);
    mLabelMessage->setStyleSheet("color: #666666;");
    mLabelMessage->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred));

    QHBoxLayout *layoutContent = new QHBoxLayout(this);
    layoutContent->setContentsMargins(mContentMargin * 2, 0, 0, 0);
    layoutContent->addWidget(lbImage);
    layoutContent->addWidget(mLabelMessage);
    widgetBase->setLayout(layoutContent);

    QGridLayout *layout = new QGridLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(widgetBase);
    setLayout(layout);

    // 动画
    mAnimation = new QPropertyAnimation(this, "pos");
    mAnimation->setDuration(240);
    mAnimation->setEasingCurve(QEasingCurve::OutSine);
    connect(mAnimation, &QPropertyAnimation::finished, this, &MessageWidget::slot_position_animation_finish);

    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect->setOffset(0, 2);
    shadowEffect->setColor(QColor(214, 214, 214));
    shadowEffect->setBlurRadius(12);
    setGraphicsEffect(shadowEffect);
}

void MessageWidget::animateDesign()
{
    if (nullptr == mParentWidget) return;
    // 计算合适大小和位置
    int parentWidth = mParentWidget->width();
    int parentHeight = mParentWidget->height();

    QFont font("Microsoft YaHei", 9);
    QFontMetrics metrics(font);

    QRect fontRect = metrics.boundingRect(mMessage);
    int width = this->height() * 3 + fontRect.width();
    resize(width, metrics.height() * 2.5);

    if (mMessagePosition == P_Top_Center)
    {
        // 在更改父组件大小的时候，动态修改消息控件位置
        setGeometry((parentWidth - width) / 2.0, this->y(), this->width(), this->height());

        mAnimation->setStartValue(QPoint((parentWidth - width) / 2.0, -fontRect.height() * 2.8));
        mAnimation->setEndValue(QPoint((parentWidth - width) / 2.0, fontRect.height() * 4.2 * mSerialNumber));
    }
    else if (mMessagePosition == P_Top_Right)
    {
        // 在更改父组件大小的时候，动态修改消息控件位置
        setGeometry((parentWidth - width - fontRect.height()), this->y(), this->width(), this->height());

        mAnimation->setStartValue(QPoint(parentWidth - width - fontRect.height(), -fontRect.height() * 2.8));
        mAnimation->setEndValue(QPoint(parentWidth - width - fontRect.height(), fontRect.height() * 4.2 * mSerialNumber));
    }
    else if (mMessagePosition == P_Bottom_Center)
    {
        // 在更改父组件大小的时候，动态修改消息控件位置
        setGeometry((parentWidth - width) / 2.0, this->y(), this->width(), this->height());

        mAnimation->setStartValue(QPoint((parentWidth - width) / 2.0, parentHeight + fontRect.height() * 2.8));
        mAnimation->setEndValue(QPoint((parentWidth - width) / 2.0, parentHeight - fontRect.height() * 4.2 * mSerialNumber));
    }
    else if (mMessagePosition == P_Bottom_Right)
    {
        // 在更改父组件大小的时候，动态修改消息控件位置
        setGeometry((parentWidth - width - fontRect.height()), parentHeight - fontRect.height() * 4.2 * mSerialNumber, this->width(), this->height());

        mAnimation->setStartValue(QPoint(parentWidth - width - fontRect.height(), parentHeight + fontRect.height() * 2.8));
        mAnimation->setEndValue(QPoint(parentWidth - width - fontRect.height(), parentHeight - fontRect.height() * 4.2 * mSerialNumber));
    }
}

void MessageWidget::slot_hide_message_widget()
{
    mListSerialNumber.removeOne(mSerialNumber);

    // 在该函数中自删除
    mAnimation->setDuration(120);
    mAnimation->setDirection(QPropertyAnimation::Backward);
    mAnimation->start();
}

void MessageWidget::slot_position_animation_finish()
{
    if (mAnimation->direction() == QPropertyAnimation::Backward)
    {
        setVisible(false);
        delete mAnimation;
        delete this;
    }
    else
    {
        QTimer::singleShot(3200, this, &MessageWidget::slot_hide_message_widget);
    }
}

int MessageWidget::getMessagePosition() const
{
    return mMessagePosition;
}

void MessageWidget::setMessagePosition(int newMessagePosition)
{
    mMessagePosition = newMessagePosition;
}

int MessageWidget::getMessageType() const
{
    return mMessageType;
}

void MessageWidget::setMessageType(int newMessageType)
{
    mMessageType = newMessageType;
}

int MessageWidget::getMaxMessageNumber() const
{
    return mMaxMessageNumber;
}

void MessageWidget::setMaxMessageNumber(int newMaxMessageNumber)
{
    mMaxMessageNumber = newMaxMessageNumber;
}

void MessageWidget::setColor(qreal r, qreal g, qreal b, qreal a)
{
    mColor = QColor(r, g, b, a);
}

void MessageWidget::setBorderColor(qreal r, qreal g, qreal b, qreal a)
{
    mBorderColor = QColor(r, g, b, a);
}

void MessageWidget::setMessage(const QString &message)
{
    mMessage = message;
}
