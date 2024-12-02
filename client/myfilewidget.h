#ifndef MYFILEWIDGET_H
#define MYFILEWIDGET_H

#include <QWidget>

namespace Ui {
class MyFileWidget;
}

class MyFileWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MyFileWidget(QWidget *parent = nullptr);
    ~MyFileWidget();

private:
    Ui::MyFileWidget *ui;
};

#endif // MYFILEWIDGET_H
