#include "login.h"
#include "ui_login.h"

#include "widgetlogintitle.h"
#include "common/config.h"
#include "structs/userinfo.h"
#include "common/jsontool.h"
#include "common/networktool.h"
#include "structs/httpreplaycode.h"


#include <QPainter>
#include <QRegularExpression>
#include <QMessageBox>
#include <QJsonDocument>

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

Login::Login(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Login)
{
    ui->setupUi(this);
    ui->swLoginPages->setCurrentWidget(ui->pageLogin);

    this->setWindowIcon(QIcon(":/img/foxcloud-logo.svg"));

    /* 从配置问价加载 webserver 信息 */
    WebServerInfo serverInfo = JsonTool::getWebServerInfo(PATH_FOXCLOUD_CLIENT_CONFIG);
    ui->lePageServerAddress->setText(serverInfo.address);
    ui->lePageServerPort->setText(QString::number(serverInfo.port));

    ui->lePageLoginPwd->setEchoMode(QLineEdit::Password);
    ui->lePageRegPwd->setEchoMode(QLineEdit::Password);
    ui->lePageRegConfirmPwd->setEchoMode(QLineEdit::Password);

    /* 点击服务器设置页面 */
    connect(ui->wgTitle, &WidgetLoginTitle::showSetServerPage, this, [=](){
        ui->swLoginPages->setCurrentWidget(ui->pageSetServer);
        ui->lePageServerAddress->setFocus();  // 设置焦点
    });

    /* 点击前往注册 */
    connect(ui->btnTurnToRegPage, &QPushButton::clicked, this, [=](){
        ui->swLoginPages->setCurrentWidget(ui->pageReg);
        ui->lePageRegLogin->setFocus();  // 设置焦点
    });

    /* 点击注册按钮 */
    connect(ui->btnReg, &QPushButton::clicked, this, &Login::registerUser);

    /* 点击关闭按钮时候窗口相互的切换 */
    connect(ui->wgTitle, &WidgetLoginTitle::closeCurPage, this, [=](){
        if (ui->swLoginPages->currentWidget() == ui->pageSetServer ||
            ui->swLoginPages->currentWidget() == ui->pageReg)
        {
            ui->swLoginPages->setCurrentWidget(ui->pageLogin);
            ui->lePageLoginLogin->setFocus();
        }
        else
        {
            this->close();
        }
    });
}

Login::~Login()
{
    delete ui;
}

/** 绘图事件函数（这是一个回调函数，每当窗口刷新的时候自动调用）：给窗口添加背景图
 * @brief Login::paintEvent
 * @param event
 */
void Login::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    QPixmap pixmap(":/img/background-blud-prink.jpg");
    painter.drawPixmap(0, 0, this->width(), this->height(), pixmap);
}

bool Login::registerUser()
{
    UserInfo user;
    user.login       = ui->lePageRegLogin->text();
    user.nickname    = ui->lePageRegNickname->text();
    user.password    = ui->lePageRegPwd->text();
    user.phone       = ui->lePageRegPhone->text();
    user.email       = ui->lePageRegEmail->text();

    WebServerInfo serverInfo;
    serverInfo.address  = ui->lePageServerAddress->text();
    serverInfo.port     = (qint16)ui->lePageServerPort->text().toInt();


    /* 利用正则表达式校验数据 */
    QRegularExpression reg;
    reg.setPattern(REG_LOGIN);
    if (!reg.match(user.login).hasMatch())
    {
        QMessageBox::warning(this ,"Warning","Incorrect login format, only upper and lower case letters and underscores are allowed");
        ui->lePageRegLogin->clear();
        ui->lePageRegLogin->setFocus();
        return false;
    }

    if (user.nickname.length() < 1)
    {
        QMessageBox::warning(this, "Warning", "Nickname length needs to be greater than 1");
        return false;
    }

    reg.setPattern(REG_PASSWD);
    if (!reg.match(user.password).hasMatch())
    {
        QMessageBox::warning(this ,"Warning","Incorrect password format, only upper and lower case letters and underscores are allowed");
        ui->lePageRegPwd->clear();
        ui->lePageRegPwd->setFocus();
        return false;
    }

    if (user.password != ui->lePageRegConfirmPwd->text())
    {
        QMessageBox::warning(this, "Warning", "Inconsistent passwords entered twice");
        ui->lePageRegPwd->clear();
        ui->lePageRegConfirmPwd->clear();
        ui->lePageRegPwd->setFocus();
        return false;
    }

    reg.setPattern(REG_PHONE);
    if (!reg.match(user.phone).hasMatch())
    {
        QMessageBox::warning(this ,"Warning","Incorrect phone number format, it should be 6 to 10 digits");
        ui->lePageRegPhone->clear();
        ui->lePageRegPhone->setFocus();
        return false;
    }

    reg.setPattern(REG_EMAIL);
    if (!reg.match(user.email).hasMatch())
    {
        QMessageBox::warning(this ,"Warning","Incorrect email format");
        ui->lePageRegEmail->clear();
        ui->lePageRegEmail->setFocus();
        return false;
    }

    /* 注册信息转为 JSON */
    QByteArray jsonPostData = JsonTool::getRegistrationJsonForServer(user);

    /* 发送 Http 请求协议 */
    QNetworkRequest request;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setHeader(QNetworkRequest::ContentLengthHeader, jsonPostData.size());

    QString url = QString("http://%1:%2/reg").arg(serverInfo.address, QString::number(serverInfo.port));
    request.setUrl(url);
    qDebug() << "Registration info will send to URL:" << url << "with data: " << jsonPostData;

    QNetworkAccessManager* manager = NetworkTool::getNetworkManager();
    QNetworkReply* replay = manager->post(request, jsonPostData);  // 发送请求
    connect(replay, &QNetworkReply::readyRead, this, [=](){
        /*
         * 读取返回的数据
         * 成功 {"code":"002"}
         * 用户已存在 {"code":"003"}
         * 失败 {"code":"004"}
        */
        QByteArray replayData = replay->readAll();
        const QString code = NetworkTool::getReplayCode(replayData);

        if (code == HttpReplayCode::SUCCESS)
        {
            /* 帮用户把成功的信息填到输入框，然后跳转 */
            ui->lePageLoginLogin->setText(user.login);
            ui->lePageLoginPwd->setText(user.password);
            ui->swLoginPages->setCurrentWidget(ui->pageLogin);

            /* 保存信息 */
            JsonTool::overwriteUserInfo(user, PATH_FOXCLOUD_CLIENT_CONFIG);

            QMessageBox::information(this, "Registration", "Successful registration!");
            return;
        }
        else if (code == HttpReplayCode::USER_EXISIT)
        {
            QMessageBox::warning(this, "Warning", QString("User with login %1 already exists").arg(user.login));
            return;
        }
        else if (code == HttpReplayCode::FAIL)
        {
            QMessageBox::critical(this, "Error", "Can not  registration!");
            return;
        }
        else
        {
            QMessageBox::critical(this, "Error", QString("Can not registration, server replay code: %1").arg(code));
            return;
        }
    });


    return true;
}

bool Login::connectServer()
{
    QString ip = ui->lePageServerAddress->text();
    QString port = ui->lePageServerPort->text();

    /* 利用正则表达式校验数据 */
    QRegularExpression reg;
    reg.setPattern(REG_IP);
    if (!reg.match(ip).hasMatch())
    {
        QMessageBox::warning(this ,"Warning","Incorrect IP format");
        ui->lePageServerAddress->clear();
        ui->lePageServerAddress->setFocus();
        return false;
    }

    reg.setPattern(REG_PORT);
    if (!reg.match(port).hasMatch())
    {
        QMessageBox::warning(this ,"Warning","Incorrect port format");
        ui->lePageServerPort->clear();
        ui->lePageServerPort->setFocus();
        return false;
    }


// TODO
    return true;
}



