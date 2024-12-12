#include "windowmanager.h"

#include <QDebug>
#include <QFontDatabase>

#include "common/logger.h"
#include "common/clientinfoinstance.h"


WindowManager::WindowManager(QObject* parent) :
    QObject{parent},
    _login(nullptr),
    _mainwindow(nullptr)
{
    qInfo() << "Start init Window manager";

    initLogin();

    connectSingalSlot();
}

WindowManager::~WindowManager()
{
    qInfo() << "Destory Window manager";
    qInfo() << "=================================================================================";

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
 * @brief WindowManager::initLogin 初始化 Login 窗口
 */
void WindowManager::initLogin()
{
    destroyLogin();
    _login = new Login(nullptr);
}

void WindowManager::destroyLogin()
{
    if (nullptr == _login) return;

    qWarning() << "Destory Login window";
    _login->close();
    _login->deleteLater();
    delete _login;
    _login = nullptr;
}


/**
 * @brief WindowManager::initMainWindow 初始化主窗口
 * @param clientInfo
 * @param token
 */
void WindowManager::initMainWindow()
{
    destroyMainWindow();
    _mainwindow = new MainWindow(nullptr);
}

void WindowManager::destroyMainWindow()
{
    if (nullptr == _mainwindow) return;

    qWarning() << "Destory MainWindow";
    _mainwindow->close();
    _mainwindow->deleteLater();
    delete _mainwindow;
    _mainwindow = nullptr;
}


/**
 * @brief WindowManager::showLogin 初始化并显示 Login 窗口
 */
void WindowManager::showLogin()
{
    if (_login == nullptr)
    {
        qWarning() << "Login do not init, can not show";
        return;
    }

    _login->show();
}


/**
 * @brief WindowManager::showMainWindow 初始化并显示主窗口
 */
void WindowManager::showMainWindow()
{
    if (_mainwindow == nullptr)
    {
        qWarning() << "MainWindow do not init, can not show";
        return;
    }

    _mainwindow->show();
}


/**
 * @brief WindowManager::connectSingalSlot 连接两个对象间的信号和槽
 */
void WindowManager::connectSingalSlot()
{
    qInfo() << "Start connect singals and slots between windows";

    /* 初始化主窗口并显示 */
    connect(_login, &Login::logined, this, &WindowManager::onLogined);

    qInfo() << "Connected singals and slots between windows";
}

void WindowManager::onLogined(const FoxcloudClientInfo& clientInfo, const QString& token)
{
    ClientInfoInstance::getInstance()->setClientInfo(clientInfo, token);
    initMainWindow();
    showMainWindow();
}




