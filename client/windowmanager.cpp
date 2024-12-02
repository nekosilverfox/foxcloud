#include "windowmanager.h"

#include <QDebug>

#include "common/logger.h"


WindowManager::WindowManager(QObject* parent) :
    QObject{parent}, _login(nullptr), _mainwindow(nullptr)
{
    qInfo() << "Start init Window manager";

    connectSingalSlot();
}

WindowManager::~WindowManager()
{
    qInfo() << "Destory Window manager";

    if (_login != nullptr)
    {
        _login->close();
        delete _login;
        _login = nullptr;
    }

    if (_mainwindow != nullptr)
    {
        _mainwindow->close();
        delete _mainwindow;
        _mainwindow = nullptr;
    }

    Logger::clean();
}


/**
 * @brief WindowManager::getManger 用于获取唯一的 manger 对象
 * @return 唯一的 manger 对象
 */
WindowManager& WindowManager::getManger()
{
    static WindowManager manger;
    return manger;
}

/**
 * @brief WindowManager::showLogin 初始化并显示 Login 窗口
 */
void WindowManager::showLogin()
{
    if (!_login)
    {
        _login = new Login();
        qInfo() << "Init Login window";
    }

    _login->show();
}

/**
 * @brief WindowManager::showMainwindow 初始化并显示主窗口
 */
void WindowManager::showMainwindow()
{
    if (_mainwindow)
    {
        _mainwindow = new MainWindow();
        qInfo() << "Init Main window";
    }

    _mainwindow->show();
}


/**
 * @brief WindowManager::connectSingalSlot 连接两个对象间的信号和槽
 */
void WindowManager::connectSingalSlot()
{
    qInfo() << "Start connect singals and slots between windows";
}




