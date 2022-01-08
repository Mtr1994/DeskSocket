#ifndef DIALOGNOTE_H
#define DIALOGNOTE_H

#include <QDialog>
#include <QMouseEvent>
#include <QPoint>
#include <QPropertyAnimation>
#include <QEvent>

namespace Ui {
class DialogNote;
}

class DialogNote : public QDialog
{
    Q_OBJECT

public:
    explicit DialogNote(QWidget *parent = nullptr);
    ~DialogNote();
    void setTitle(const QString& title);
    ///
    /// \brief setMessage
    /// \param message
    /// 设置消息内容并展示，可支持 Html 格式
    ///
    void showMessage(const QString& message);
    void hideButtonCancel();

protected:
    bool event(QEvent* event) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent*event) override;

private slots:
    void on_btnConfirm_clicked();

    void on_btnCancel_clicked();

private:
    void setStyle();
    void initAnimation();

private:
    Ui::DialogNote *ui;
    bool mLeftBtnClk = false;
    QPoint mPress;
    QPoint mMove;

    //
    QPropertyAnimation *mMoveingAnimation = nullptr;
};

#endif // DIALOGNOTE_H
