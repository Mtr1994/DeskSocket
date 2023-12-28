#ifndef WIDGETAPPSETTING_H
#define WIDGETAPPSETTING_H

#include <QWidget>

namespace Ui {
class WidgetAppSetting;
}

class WidgetAppSetting : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetAppSetting(QWidget *parent = nullptr);
    ~WidgetAppSetting();

private:
    void init();

    void slot_btn_ok_click();

    void slot_btn_reset_click();

private:
    Ui::WidgetAppSetting *ui;
};

#endif // WIDGETAPPSETTING_H
