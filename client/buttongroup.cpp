#include "buttongroup.h"
#include "ui_buttongroup.h"

#include <QPushButton>

ButtonGroup::ButtonGroup(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ButtonGroup)
{
    ui->setupUi(this);

    ui->lbLogo->setPixmap(QPixmap(":/img/foxcloud-logo-name.svg").scaled(QSize(160, 85), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    // ui->btnCurrentUser->setIcon(QIcon(":/img/circle-user.svg"));

    ui->btnShareList->hide();
    ui->btnTopDownload->hide();

    connect(ui->btnCurrentUser, &QPushButton::clicked, [=](){emit ButtonGroup::onBtnCurrentUserClicked();});
    connect(ui->btnMyfile, &QPushButton::clicked, [=](){emit ButtonGroup::onBtnMyFilesClicked();});
    connect(ui->btnShareList, &QPushButton::clicked, [=](){emit ButtonGroup::onBtnShareListClicked();});
    connect(ui->btnTopDownload, &QPushButton::clicked, [=](){emit ButtonGroup::onBtnTopDownloadClicked();});
    connect(ui->btnTransportList, &QPushButton::clicked, [=](){emit ButtonGroup::onBtnTransportListClicked();});
    connect(ui->btnLogout, &QPushButton::clicked, [=](){emit ButtonGroup::onBtnLogoutClicked();});
}

ButtonGroup::~ButtonGroup()
{
    delete ui;
}

void ButtonGroup::setCurrentUserLogin(const QString &login)
{
    if (login.isEmpty()) return;

    ui->btnCurrentUser->setText(login);
}
