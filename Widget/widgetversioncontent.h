#ifndef WIDGETVERSIONCONTENT_H
#define WIDGETVERSIONCONTENT_H

#include <QWidget>

namespace Ui {
class WidgetVersionContent;
}

class WidgetVersionContent : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetVersionContent(QWidget *parent = nullptr);
    ~WidgetVersionContent();

private:
    void init();

private:
    Ui::WidgetVersionContent *ui;
};

#endif // WIDGETVERSIONCONTENT_H
