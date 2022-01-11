#ifndef DIALOGFORMATJSON_H
#define DIALOGFORMATJSON_H

#include <QDialog>

namespace Ui {
class DialogFormatJson;
}

class DialogFormatJson : public QDialog
{
    Q_OBJECT

public:
    explicit DialogFormatJson(QWidget *parent = nullptr);
    ~DialogFormatJson();

    void init();

private:
    void slot_format_click();
    void slot_copy_click();

    void slot_text_value_change();

private:
    Ui::DialogFormatJson *ui;
};

#endif // DIALOGFORMATJSON_H
