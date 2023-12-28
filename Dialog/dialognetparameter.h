#ifndef DIALOGNETPARAMETER_H
#define DIALOGNETPARAMETER_H

#include <QDialog>

namespace Ui {
class DialogNetParameter;
}

class DialogNetParameter : public QDialog
{
    Q_OBJECT

public:
    explicit DialogNetParameter(int type, const QString& title, QWidget *parent = nullptr);
    ~DialogNetParameter();

private:
    void init(const QString& title);

public slots:
    void slot_btn_cancel_clicked();

    void slot_btn_confirm_clicked();

private:
    Ui::DialogNetParameter *ui;

    int mNetworkObjectType = -1;
};

#endif // DIALOGNETPARAMETER_H
