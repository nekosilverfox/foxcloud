#include "buttongroup.h"
#include "ui_buttongroup.h"

ButtonGroup::ButtonGroup(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ButtonGroup)
{
    ui->setupUi(this);

    ui->lbLogo->setPixmap(QPixmap(":/img/foxcloud-logo-name.svg").scaled(QSize(160, 85), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    // ui->btnCurrentUser->setIcon(QIcon(":/img/circle-user.svg"));
}

ButtonGroup::~ButtonGroup()
{
    delete ui;
}
