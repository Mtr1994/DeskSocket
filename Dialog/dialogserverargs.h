#ifndef DIALOGSERVERARGS_H
#define DIALOGSERVERARGS_H

#include <QDialog>

namespace Ui {
class DialogServerArgs;
}

class DialogServerArgs : public QDialog
{
    Q_OBJECT

public:
    explicit DialogServerArgs(const QString &title, QWidget *parent = nullptr);
    ~DialogServerArgs();

    QString getAddress();
    QString getPort();

private slots:
    void on_btnCancel_clicked();

    void on_btnConfirm_clicked();

private:
    Ui::DialogServerArgs *ui;
};

#endif // DIALOGSERVERARGS_H
