#include "myfilewidget.h"
#include "ui_myfilewidget.h"

MyFileWidget::MyFileWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MyFileWidget)
{
    ui->setupUi(this);
}

MyFileWidget::~MyFileWidget()
{
    delete ui;
}
