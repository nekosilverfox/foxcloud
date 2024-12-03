#include "transportbar.h"
#include "ui_transportbar.h"

TransportBar::TransportBar(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TransportBar)
{
    ui->setupUi(this);
}

TransportBar::~TransportBar()
{
    delete ui;
}
