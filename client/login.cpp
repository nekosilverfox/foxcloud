#include "login.h"
#include "ui_login.h"

#include "widgetlogintitle.h"
#include "common/config.h"

#include <QPainter>
#include <QRegularExpression>
#include <QMessageBox>

Login::Login(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Login)
{
    ui->setupUi(this);
    ui->swLoginPages->setCurrentWidget(ui->pageLogin);

    ui->lePageLoginPwd->setEchoMode(QLineEdit::Password);
    ui->lePageRegPwd->setEchoMode(QLineEdit::Password);
    ui->lePageRegConfirmPwd->setEchoMode(QLineEdit::Password);

    /* 点击服务器设置页面 */
    connect(ui->wgTitle, &WidgetLoginTitle::showSetServerPage, this, [=](){
        ui->swLoginPages->setCurrentWidget(ui->pageSetServer);
        ui->lePageServerIp->setFocus();  // 设置焦点
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
void Login::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPixmap pixmap(":/img/background-blud-prink.jpg");
    painter.drawPixmap(0, 0, this->width(), this->height(), pixmap);
}

bool Login::registerUser()
{
    QString login       = ui->lePageRegLogin->text();
    QString nickname    = ui->lePageRegNickname->text();
    QString pwd         = ui->lePageRegPwd->text();
    QString configPwd   = ui->lePageRegConfirmPwd->text();
    QString phone       = ui->lePageRegPhone->text();
    QString email       = ui->lePageRegEmail->text();

    /* 利用正则表达式校验数据 */
    QRegularExpression reg;
    reg.setPattern(REG_LOGIN);
    if (!reg.match(login).hasMatch())
    {
        QMessageBox::warning(this ,"Warning","Incorrect login format, only upper and lower case letters and underscores are allowed");
        ui->lePageRegLogin->clear();
        ui->lePageRegLogin->setFocus();
        return false;
    }

    if (nickname.length() < 1)
    {
        QMessageBox::warning(this, "Warning", "Nickname length needs to be greater than 1");
        return false;
    }

    reg.setPattern(REG_PASSWD);
    if (!reg.match(pwd).hasMatch())
    {
        QMessageBox::warning(this ,"Warning","Incorrect password format, only upper and lower case letters and underscores are allowed");
        ui->lePageRegPwd->clear();
        ui->lePageRegPwd->setFocus();
        return false;
    }

    if (pwd != configPwd)
    {
        QMessageBox::warning(this, "Warning", "Inconsistent passwords entered twice");
        ui->lePageRegPwd->clear();
        ui->lePageRegConfirmPwd->clear();
        ui->lePageRegPwd->setFocus();
        return false;
    }

    reg.setPattern(REG_PHONE);
    if (!reg.match(phone).hasMatch())
    {
        QMessageBox::warning(this ,"Warning","Incorrect phone number format, it should be 6 to 10 digits");
        ui->lePageRegPhone->clear();
        ui->lePageRegPhone->setFocus();
        return false;
    }

    reg.setPattern(REG_EMAIL);
    if (!reg.match(email).hasMatch())
    {
        QMessageBox::warning(this ,"Warning","Incorrect email format");
        ui->lePageRegEmail->clear();
        ui->lePageRegEmail->setFocus();
        return false;
    }



}

bool Login::connectServer()
{
    QString ip = ui->lePageServerIp->text();
    QString port = ui->lePageServerPort->text();

    /* 利用正则表达式校验数据 */
    QRegularExpression reg;
    reg.setPattern(REG_IP);
    if (!reg.match(ip).hasMatch())
    {
        QMessageBox::warning(this ,"Warning","Incorrect IP format");
        ui->lePageServerIp->clear();
        ui->lePageServerIp->setFocus();
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


}



