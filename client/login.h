#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();

protected:
    /* 绘图事件函数 */
    void paintEvent(QPaintEvent*) override;

private slots:
    bool registerUser();
    bool loginUser();
    bool connectServer();

private:
    Ui::Login *ui;
};

#endif // LOGIN_H
