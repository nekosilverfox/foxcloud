#include "transportwidget.h"
#include "ui_transportwidget.h"

TransportWidget::TransportWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TransportWidget)
{
    ui->setupUi(this);
}

TransportWidget::~TransportWidget()
{
    delete ui;
}
