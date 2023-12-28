#ifndef WIDGETFORMATJSON_H
#define WIDGETFORMATJSON_H

#include <QWidget>

namespace Ui {
class WidgetFormatJson;
}

class WidgetFormatJson : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetFormatJson(QWidget *parent = nullptr);
    ~WidgetFormatJson();

private:
    void init();

private slots:
    void slot_btn_format_click();
    void slot_json_custom_menu_request(const QPoint &pos);

private:
    Ui::WidgetFormatJson *ui;
};

#endif // WIDGETFORMATJSON_H
