#ifndef WIDGETLOGINTITLE_H
#define WIDGETLOGINTITLE_H

#include <QWidget>

namespace Ui {
class WidgetLoginTitle;
}

class WidgetLoginTitle : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetLoginTitle(QWidget *parent = nullptr);
    ~WidgetLoginTitle();

private:
    Ui::WidgetLoginTitle *ui;
};

#endif // WIDGETLOGINTITLE_H
