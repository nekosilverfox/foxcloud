#ifndef BUTTONGROUP_H
#define BUTTONGROUP_H

#include <QWidget>

namespace Ui {
class ButtonGroup;
}

class ButtonGroup : public QWidget
{
    Q_OBJECT

public:
    explicit ButtonGroup(QWidget *parent = nullptr);
    ~ButtonGroup();

    void setCurrentUserLogin(const QString& login);

signals:
    void onBtnCurrentUserClicked();
    void onBtnMyFilesClicked();
    void onBtnShareListClicked();
    void onBtnTopDownloadClicked();
    void onBtnTransportListClicked();
    void onBtnLogoutClicked();

private:
    Ui::ButtonGroup *ui;
};

#endif // BUTTONGROUP_H
