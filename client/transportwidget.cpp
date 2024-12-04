#include "transportwidget.h"
#include "ui_transportwidget.h"

#include "common/uploadlayout.h"

TransportWidget::TransportWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TransportWidget)
{
    ui->setupUi(this);

    /* 把文件上传的 Layout */
    UploadLayout::getInstance()->initUploadLayout(ui->saUploadWidget);

}

TransportWidget::~TransportWidget()
{
    delete ui;
}
