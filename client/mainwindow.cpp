#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPainter>

#include "structs/foxcloudclientinfo.h"
#include "buttongroup.h"
#include "common/config.h"
#include "common/logger.h"

MainWindow::MainWindow(QWidget *parent, const FoxcloudClientInfo& clientInfo, const QString& token) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _clientInfo(clientInfo),
    _token(token)
{
    ui->setupUi(this);

    qInfo() << "Init MainWindow";

    this->setWindowIcon(QIcon(":/img/foxcloud-logo.svg"));

    /* 修改控件信息 */
    ui->btnGroup->setCurrentUserLogin(clientInfo.userInfo.login);

    /* 用户信息添加到表格中 */
    QStringList clientInfoKeyList;
    clientInfoKeyList << STR_VAL_LOGIN << STR_VAL_ADDRESS << STR_VAL_PORT;

    QStringList clientInfoValueList;
    clientInfoValueList << clientInfo.userInfo.login << clientInfo.webServerInfo.address << QString::number(clientInfo.webServerInfo.port);

    ui->twClientInfo->setRowCount(clientInfoKeyList.size());
    ui->twClientInfo->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->twClientInfo->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    for (int i = 0; i < clientInfoKeyList.size(); i++)
    {
        ui->twClientInfo->setItem(i, 0, new QTableWidgetItem(clientInfoKeyList.at(i)));
        ui->twClientInfo->setItem(i, 1, new QTableWidgetItem(clientInfoValueList.at(i)));

        qDebug() << "Add to table:" << clientInfoKeyList.at(i) << clientInfoValueList.at(i);
    }

    connect(ui->btnGroup, &ButtonGroup::onBtnCurrentUserClicked, this, [=](){ ui->swPages->setCurrentWidget(ui->pUserInfo); });
    connect(ui->btnGroup, &ButtonGroup::onBtnMyFilesClicked, this, [=](){ ui->swPages->setCurrentWidget(ui->pMyFile); });
    connect(ui->btnGroup, &ButtonGroup::onBtnTransportListClicked, this, [=](){ ui->swPages->setCurrentWidget(ui->pTransport); });

    connect(ui->btnGroup, &ButtonGroup::onBtnLogoutClicked, this, [](){ emit &ButtonGroup::onBtnLogoutClicked; });  //TODO 完成退出流程，文件停止传输等
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
