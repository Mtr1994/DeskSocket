#ifndef WIDGETFORMATPOS_H
#define WIDGETFORMATPOS_H

#include <QWidget>

namespace Ui {
class WidgetFormatPos;
}

class WidgetFormatPos : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetFormatPos(QWidget *parent = nullptr);
    ~WidgetFormatPos();

private:
    void init();

private slots:
    void slot_btn_format_to_dms_click();
    void slot_btn_copy_dms_click();
    void slot_btn_format_to_pos_click();
    void slot_btn_copy_pos_click();

private:
    Ui::WidgetFormatPos *ui;

    bool mFormatPosStatus = false;
    bool mFormatDmsStatus = false;
};

#endif // WIDGETFORMATPOS_H
