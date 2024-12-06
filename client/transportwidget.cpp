#include "transportwidget.h"
#include "ui_transportwidget.h"

#include <QObject>

#include "common/uploadlayout.h"
#include "common/downloadlayout.h"

#include "mainwindow.h"

TransportWidget::TransportWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TransportWidget)
{
    ui->setupUi(this);

    /* 把文件上传使用的 Layout 初始化*/
    UploadLayout::getInstance()->initUploadLayout(ui->saUploadWidget);
    DownloadLayout::getInstance()->initDownloadLayout(ui->saDownloadWidget);

}

TransportWidget::~TransportWidget()
{
    delete ui;
}

/**
 * @brief TransportWidget::turnToTabUpload 切换到上传 Tab
 */
void TransportWidget::turnToTabUpload()
{
    ui->tabWidget->setCurrentWidget(ui->tabUpload);
}

void TransportWidget::turnToTabDownload()
{
    ui->tabWidget->setCornerWidget(ui->tabDownload);
}
