#ifndef DIALOGTCPCLIENTARGS_H
#define DIALOGTCPCLIENTARGS_H

#include <QDialog>

namespace Ui {
class DialogTcpClientArgs;
}

class DialogTcpClientArgs : public QDialog
{
    Q_OBJECT

public:
    explicit DialogTcpClientArgs(QWidget *parent = nullptr);
    ~DialogTcpClientArgs();

    QString getAddress();
    QString getPort();

private slots:
    void on_btnOk_clicked();

    void on_btnCancel_clicked();

private:
    Ui::DialogTcpClientArgs *ui;
};

#endif // DIALOGTCPCLIENTARGS_H
