#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "structs/foxcloudclientinfo.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr, const FoxcloudClientInfo& clientInfo = FoxcloudClientInfo(), const QString& token = "");
    ~MainWindow();

protected:
    /* 绘图事件函数 */
    void paintEvent(QPaintEvent*) override;

private:
    Ui::MainWindow* ui;

    FoxcloudClientInfo _clientInfo;  //当前用户配置信息
    QString            _token;       //
};
#endif // MAINWINDOW_H
