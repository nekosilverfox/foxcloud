#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPainter>

#include "buttongroup.h"
#include "common/clientinfoinstance.h"
#include "common/config.h"
#include "common/logger.h"
#include "common/uploadlayout.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qInfo() << "Init MainWindow";

    this->setWindowIcon(QIcon(":/img/foxcloud-logo.svg"));
    this->setWindowTitle("Foxcloud");

    /* 通过单例模式获取唯一的用户信息 */
    FoxcloudClientInfo clientInfo = ClientInfoInstance::getInstance()->getClientInfo();

    /* 修改控件信息 */
    ui->btnGroup->setCurrentUserLogin(clientInfo.userInfo.login);

    /* 登录后显示的主页 */
    ui->swPages->setCurrentWidget(ui->pUserInfo);

    /* 用户信息添加到表格中 */
    QStringList clientInfoKeyList;
    clientInfoKeyList << STR_VAL_LOGIN
                      << STR_VAL_ADDRESS
                      << STR_VAL_PORT
                      << "Token";

    QStringList clientInfoValueList;
    clientInfoValueList << clientInfo.userInfo.login
                        << clientInfo.webServerInfo.address
                        << QString::number(clientInfo.webServerInfo.port)
                        << ClientInfoInstance::getInstance()->getToken();

    ui->twClientInfo->setRowCount(clientInfoKeyList.size());
    ui->twClientInfo->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->twClientInfo->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    for (int i = 0; i < clientInfoKeyList.size(); i++)
    {
        ui->twClientInfo->setItem(i, 0, new QTableWidgetItem(clientInfoKeyList.at(i)));
        ui->twClientInfo->setItem(i, 1, new QTableWidgetItem(clientInfoValueList.at(i)));

        qDebug() << "Add to UserInfo Table:" << clientInfoKeyList.at(i) << clientInfoValueList.at(i);
    }

    connect(ui->btnGroup, &ButtonGroup::onBtnCurrentUserClicked, this, [=](){ ui->swPages->setCurrentWidget(ui->pUserInfo); });
    connect(ui->btnGroup, &ButtonGroup::onBtnMyFilesClicked, this, [=](){ ui->swPages->setCurrentWidget(ui->pMyFile); });
    connect(ui->btnGroup, &ButtonGroup::onBtnTransportListClicked, this, [=](){ ui->swPages->setCurrentWidget(ui->pTransport); });
    connect(ui->btnGroup, &ButtonGroup::onBtnTopDownloadClicked, this, [=](){ ui->swPages->setCurrentWidget(ui->pTopDownload); });
    connect(ui->btnGroup, &ButtonGroup::onBtnShareListClicked, this, [=](){ ui->swPages->setCurrentWidget(ui->pShare); });
    connect(ui->btnGroup, &ButtonGroup::onBtnLogoutClicked, this, [=](){
        qDebug() << "emit MainWindow::onLogoutFromMainWindow();";
        emit MainWindow::onLogoutFromMainWindow(); });  //TODO 完成退出流程，文件停止传输等

    /* 一路跳转到上传显示区域 */
    connect(ui->myFileWidget, &MyFileWidget::jumpToTabUpload, this, [=](){
        ui->swPages->setCurrentWidget(ui->pTransport);
        ui->transportWidget->turnToTabUpload();
    });

    connect(ui->myFileWidget, &MyFileWidget::jumpToTabDownload, this, [=](){
        ui->swPages->setCurrentWidget(ui->pTransport);
        ui->transportWidget->turnToTabDownload();
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}


/** 绘图事件函数（这是一个回调函数，每当窗口刷新的时候自动调用）：给窗口添加背景图
 * @brief Login::paintEvent
 * @param event
 */
void MainWindow::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    QPixmap pixmap(":/img/background-blud-prink.jpg");
    painter.drawPixmap(0, 0, this->width(), this->height(), pixmap);
}
