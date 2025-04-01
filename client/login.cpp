#include "login.h"
#include "ui_login.h"

#include "widgetlogintitle.h"
#include "common/config.h"
#include "common/jsontool.h"
#include "common/networktool.h"
#include "structs/httpreplaycode.h"
#include "common/encrypttool.h"


#include <QPainter>
#include <QRegularExpression>
#include <QMessageBox>
#include <QJsonDocument>

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

#include "structs/foxcloudclientinfo.h"

Login::Login(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Login)
{
    ui->setupUi(this);

    qInfo() << "Init Login window";

    ui->swLoginPages->setCurrentWidget(ui->pageLogin);

    ui->wSavePwd->hide();  /// TODO 加密完成后删除此

    this->setWindowIcon(QIcon(":/img/foxcloud-logo.svg"));
    this->setWindowTitle("Foxcloud login");

    /* 从配置文件加载 client 信息 */
    FoxcloudClientInfo clientInfo = JsonTool::getFoxcloudClientInfo(PATH_FOXCLOUD_CLIENT_CONFIG);
    clientInfo.webServerInfo.address.isEmpty() ?
        ui->lePageServerAddress->setText("localhost") :
        ui->lePageServerAddress->setText(clientInfo.webServerInfo.address);

    0 == clientInfo.webServerInfo.port ?
        ui->lePageServerPort->setText(QString::number(80)) :
        ui->lePageServerPort->setText(QString::number(clientInfo.webServerInfo.port));

    ui->lePageLoginLogin->setText(clientInfo.userInfo.login);
    if ("true" == clientInfo.userInfo.remember_password)
    {
        // TODO
    }

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

    /* 点击注册 */
    connect(ui->btnLogin, &QPushButton::clicked, this, &Login::loginUser);

    /* TODO 之后改掉测试连接点击 */
    connect(ui->btnConnect, &QPushButton::clicked, this, [=](){
        ui->swLoginPages->setCurrentWidget(ui->pageLogin);
    });
    connect(ui->btnBackToLogin, &QPushButton::clicked, this, [=](){
        ui->swLoginPages->setCurrentWidget(ui->pageLogin);
    });

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

    /* 检查有没有设置服务器 */
    if (ui->lePageServerAddress->text().isEmpty() || 0 == ui->lePageServerPort)
    {
        QMessageBox::warning(this, "Warning", "Please setup your Foxcloud server");
        return false;
    }

    /* 加密密码 */
    QString tmpPwd = user.password;  // 用于之后填到登录界面
    user.password = EncryptTool::getStrMD5(user.password).toBase64();

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
    QNetworkReply* reply = manager->post(request, jsonPostData);  // 发送请求
    connect(reply, &QNetworkReply::readyRead, this, [=](){
        if (reply->error() != QNetworkReply::NoError)
        {
            qCritical() << reply->errorString();
            reply->deleteLater();  // 释放资源
            return;
        }

        /*
         * 读取返回的数据
         * 成功 {"code":"002"}
         * 用户已存在 {"code":"003"}
         * 失败 {"code":"004"}
        */
        QByteArray replayData = reply->readAll();
        reply->deleteLater();  // 释放资源
        const QString code = NetworkTool::getReplayCode(replayData);

        if (code == HttpReplayCode::Reg::SUCCESS)
        {
            /* 帮用户把成功的信息填到输入框，然后跳转 */
            ui->lePageLoginLogin->setText(user.login);
            ui->lePageLoginPwd->setText(tmpPwd);
            ui->swLoginPages->setCurrentWidget(ui->pageLogin);

            /* 保存信息 */
            JsonTool::overwriteUserInfo(user, PATH_FOXCLOUD_CLIENT_CONFIG);
            QMessageBox::information(this, "Registration", "Successful registration!");
            return;
        }
        else if (code == HttpReplayCode::Reg::USER_EXISIT)
        {
            QMessageBox::warning(this, "Warning", QString("User with login %1 already exists").arg(user.login));
            return;
        }
        else if (code == HttpReplayCode::Reg::FAIL)
        {
            QMessageBox::critical(this, "Error", "Can not  registration!");
            return;
        }
        else
        {
            QMessageBox::critical(this, "Error", QString("Can not registration, server reply code: %1 (%2)").arg(code, replayData));
            return;
        }
    });

    return true;
}

/**
 * @brief Login::loginUser 用户登录
 * @return
 */
bool Login::loginUser()
{
    FoxcloudClientInfo clientInfo = JsonTool::getFoxcloudClientInfo(PATH_FOXCLOUD_CLIENT_CONFIG);
    clientInfo.userInfo.login = ui->lePageLoginLogin->text();
    clientInfo.userInfo.password = ui->lePageLoginPwd->text();
    clientInfo.webServerInfo.address = ui->lePageServerAddress->text();
    clientInfo.webServerInfo.port = (qint16)ui->lePageServerPort->text().toInt();

    /* 检查有没有设置服务器 */
    if (ui->lePageServerAddress->text().isEmpty() || 0 == ui->lePageServerPort)
    {
        QMessageBox::warning(this, "Warning", "Please setup your Foxcloud server");
        return false;
    }

    /* 校验数据 */
    if (clientInfo.userInfo.login.length() == 0 || clientInfo.userInfo.password.length() == 0)
    {
        QMessageBox::warning(this, "Warning", "Login or password is empty");
        return false;
    }

    QRegularExpression reg;
    reg.setPattern(REG_LOGIN);
    if (!reg.match(clientInfo.userInfo.login).hasMatch())
    {
        QMessageBox::warning(this ,"Warning","Incorrect login format, only upper and lower case letters and underscores are allowed");
        ui->lePageRegLogin->clear();
        ui->lePageRegLogin->setFocus();
        return false;
    }

    reg.setPattern(REG_PASSWD);
    if (!reg.match(clientInfo.userInfo.password).hasMatch())
    {
        QMessageBox::warning(this ,"Warning","Incorrect password format, only upper and lower case letters and underscores are allowed");
        ui->lePageRegPwd->clear();
        ui->lePageRegPwd->setFocus();
        return false;
    }


    // TODO 加密用户名称
    /* 加密用户名和密码 */
    clientInfo.userInfo.password = EncryptTool::getStrMD5(clientInfo.userInfo.password).toBase64();

    /* 注册信息转为 JSON */
    QByteArray jsonPostData = JsonTool::getLoginJsonForServer(clientInfo.userInfo);

    /* 发送 Http 请求协议 */
    QNetworkRequest request;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setHeader(QNetworkRequest::ContentLengthHeader, jsonPostData.size());

    QString url = QString("http://%1:%2/login").arg(clientInfo.webServerInfo.address,
                                                    QString::number(clientInfo.webServerInfo.port));
    request.setUrl(url);
    qDebug() << "Login info will send to URL:" << url << "with data: " << jsonPostData;

    QNetworkAccessManager* manager = NetworkTool::getNetworkManager();
    QNetworkReply* reply = manager->post(request, jsonPostData);  // 发送请求
    qInfo() << "Login Json data POST to server finish";


    /* 用户没有选中保存密码 */
    if (ui->cbSavePwd->isChecked())
    {
        clientInfo.userInfo.password = ui->lePageLoginPwd->text().toUtf8().toBase64();
        clientInfo.userInfo.remember_password = "true";
    }
    else
    {
        clientInfo.userInfo.password = "";
        clientInfo.userInfo.remember_password = "false";
    }

    /* 接收并处理服务器发回的http响应消息，注意 lambda 表达式中如果使用 [&] 将会使reply释放而造成程序崩溃 */
    connect(reply, &QNetworkReply::finished, this, [=](){
        qInfo() << "Get LOGIN replay from server";

        if (reply->error() != QNetworkReply::NoError || !reply->isOpen())
        {
            qCritical() << "Network reply error:" << reply->errorString();
            reply->deleteLater();
            return;
        }

        /*  将server回写的数据读出
            登陆 - 服务器回写的json数据包格式：
            成功：{"code":"000"}
            失败：{"code":"001"}
        */
        QByteArray replayData = reply->readAll();
        reply->deleteLater();  // 释放资源
        qInfo() << "Replay LOGIN data from server:" << replayData;

        if (HttpReplayCode::Login::SUCCESS != NetworkTool::getReplayCode(replayData))
        {
            qWarning() << "Can not login";
            QMessageBox::warning(this, "Warning", "Can not login, Incorrect username or password");
            return;
        }

        //TODO 登陆成功后调用的东西
        // QMessageBox::information(this, "Login", "Successed login!");
        JsonTool::overwriteFoxcloudClientInfo(clientInfo);

        //Token 很重要，之后所有的传输都需要他，用于确认身份
        QString token = NetworkTool::getReplayToken(replayData);
        emit logined(clientInfo, token);

        this->close();
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


// TODO 增加连接ping服务器的测试方法

    return true;
}



