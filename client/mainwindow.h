#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

protected:
    /* 绘图事件函数 */
    void paintEvent(QPaintEvent*) override;

signals:
    void onLogoutFromMainWindow();

private:
    Ui::MainWindow* ui;
};
#endif // MAINWINDOW_H
