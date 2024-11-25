#ifndef WIDGETLOGINTITLE_H
#define WIDGETLOGINTITLE_H

#include <QWidget>
#include <QMouseEvent>
#include <QPoint>

namespace Ui {
class WidgetLoginTitle;
}

/**
 * @brief 作为登录窗口上面的小横条
 */
class WidgetLoginTitle : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetLoginTitle(QWidget *parent = nullptr);
    ~WidgetLoginTitle();

protected:
    /* 重写鼠标事件 */
    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

signals:
    void showSetServerPage();
    void closeCurPage();

private:
    Ui::WidgetLoginTitle *ui;
    QWidget* _parent;  // 当前插件所属的父窗口
    QPoint _pt;  // 差值：鼠标位置 - 窗口左上角
};

#endif // WIDGETLOGINTITLE_H
