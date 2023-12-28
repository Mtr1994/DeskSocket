#ifndef MESSAGEWIDGET_H
#define MESSAGEWIDGET_H

#include <QWidget>
#include <QPaintEvent>
#include <QColor>
#include <QPropertyAnimation>
#include <QList>

class QLabel;
class MessageWidget : public QWidget
{
    Q_OBJECT
public:
    enum Message_Type{ M_Success = 1, M_Error, M_Info };
    enum Message_Position { P_Top_Center = 1, P_Top_Right, P_Bottom_Center, P_Bottom_Right };
    explicit MessageWidget(int type = M_Success, int position = P_Top_Center, QWidget *parent = nullptr);

    void showMessage();
    void showMessage(const QString &message);

public:
    void setColor(qreal r, qreal g, qreal b, qreal a);
    void setBorderColor(qreal r, qreal g, qreal b, qreal a);
    void setMessage(const QString &message);

    int getMaxMessageNumber() const;
    void setMaxMessageNumber(int newMaxMessageNumber);

    int getMessageType() const;
    void setMessageType(int newMessageType);

    int getMessagePosition() const;
    void setMessagePosition(int newMessagePosition);

private:
    void init();

    void animateDesign();

private:
    void slot_hide_message_widget();
    void slot_position_animation_finish();

private:
    QWidget *mParentWidget = nullptr;

    QColor mColor = {254, 254, 254, 255};
    QColor mBorderColor = {240, 240, 240, 255};

    QString mMessage;

    QPropertyAnimation *mAnimation = nullptr;

    static QList<int> mListSerialNumber;
    // 当前消息的序号
    int mSerialNumber = -1;

    // 边框及间距
    uint32_t mContentMargin = 6;

    int mMaxMessageNumber = 8;

    int mMessageType = M_Success;
    int mMessagePosition = P_Top_Center;

    QLabel *mLabelMessage = nullptr;
};

#endif // MESSAGEWIDGET_H
