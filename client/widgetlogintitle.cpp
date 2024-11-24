#include "widgetlogintitle.h"
#include "ui_widgetlogintitle.h"

WidgetLoginTitle::WidgetLoginTitle(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WidgetLoginTitle)
{
    ui->setupUi(this);

    /* Init imgs */
    ui->lbLogo->setPixmap(QPixmap(":/img/foxcloud-logo.svg"));
    ui->lbLogo->setScaledContents(true);
    ui->lbLogo->setMaximumSize(60, 40);
    ui->btnSetServer->setIcon(QIcon(QPixmap(":/img/setting-server.svg")));
    ui->btnHide->setIcon(QIcon(QPixmap(":/img/window-minimize.svg")));
    ui->btnClose->setIcon(QIcon(QPixmap(":/img/circle-xmark.svg")));
}

WidgetLoginTitle::~WidgetLoginTitle()
{
    delete ui;
}
