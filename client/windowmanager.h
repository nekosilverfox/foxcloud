#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include <QObject>

#include "login.h"
#include "mainwindow.h"

/**
 * @brief 用于管理和调度窗口及实现窗口之间通信的消息总线
 */
class WindowManager : public QObject
{
    Q_OBJECT
public:
    static WindowManager& getManger();

    void showLogin();
    void showMainwindow();

private:
    explicit WindowManager(QObject *parent = nullptr);
    ~WindowManager();

    void connectSingalSlot();

private:
    Login*      _login;
    MainWindow* _mainwindow;
};

#endif // WINDOWMANAGER_H
