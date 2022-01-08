#ifndef DIALOGTCPSERVERARGS_H
#define DIALOGTCPSERVERARGS_H

#include <QDialog>

namespace Ui {
class DialogTcpServerArgs;
}

class DialogTcpServerArgs : public QDialog
{
    Q_OBJECT

public:
    explicit DialogTcpServerArgs(QWidget *parent = nullptr);
    ~DialogTcpServerArgs();

    QString getAddress();
    QString getPort();

private slots:
    void on_btnCancel_clicked();

    void on_btnConfirm_clicked();

private:
    Ui::DialogTcpServerArgs *ui;
};

#endif // DIALOGTCPSERVERARGS_H
