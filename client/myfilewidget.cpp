#include "myfilewidget.h"
#include "ui_myfilewidget.h"

#include <QFileDialog>
#include <QMessageBox>

#include "structs/fileinfo.h"
#include "common/uploadqueue.h"

MyFileWidget::MyFileWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MyFileWidget)
{
    ui->setupUi(this);

    /* 初始化文件显示控件 */
    initListWidgetFiles();

    /* 开始定时触发定时器，以便检查上传/下载任务队列 */
    startCheckTransportQueue(500);

    connect(ui->btnUpload, &QPushButton::clicked, this, &MyFileWidget::selectUploadFilesAndAppendToQueue);
}

MyFileWidget::~MyFileWidget()
{
    delete ui;
}

/**
 * @brief MyFileWidget::initListWidgetFiles 初始化文件列表
 */
void MyFileWidget::initListWidgetFiles()
{
    ui->lwFiles->setViewMode(QListView::IconMode);   //设置显示图标模式
    ui->lwFiles->setIconSize(QSize(50, 50));         //设置图标大小
    ui->lwFiles->setGridSize(QSize(80, 80));       //设置item大小

    // 设置QLisView大小改变时，图标的调整模式，默认是固定的，可以改成自动调整
    ui->lwFiles->setResizeMode(QListView::Adjust);   //自动适应布局

    // 设置列表可以拖动，如果想固定不能拖动，使用QListView::Static
    ui->lwFiles->setMovement(QListView::Static);
    // 设置图标之间的间距, 当setGridSize()时，此选项无效
    ui->lwFiles->setSpacing(20);

    // TODO 右键菜单
}

/**
 * @brief MyFileWidget::startCheckTransportQueue 启动用于定时检查上传/下载任务队列的定时器
 * @param interval 检查时间间隔毫秒（ms）
 */
void MyFileWidget::startCheckTransportQueue(size_t interval)
{
    // TODO
    connect(&_transportChecker, &QTimer::timeout, this, [](){});
    connect(&_transportChecker, &QTimer::timeout, this, [](){});

    _transportChecker.start(interval);  // 启动定时器以便触发定时检查
}

void MyFileWidget::selectUploadFilesAndAppendToQueue()
{
    /* 获取需要上传的文件 */
    QStringList uploadFiles = QFileDialog::getOpenFileNames(this, "Select one or more files to upload", QDir::homePath());
    qInfo() << "Get upload files:" << uploadFiles;
    for (QString path : uploadFiles)
    {
        TransportStatus status = UploadQueue::getInstance()->appendUploadFile(path);
        switch (status)
        {
        case TransportStatus::ALREADY_IN_QUEUE:
            qWarning() << "Failed add to queue, file" << path << "aleady in upload queue";
            break;

        case TransportStatus::OPEN_FAILED:
            qCritical() << "Can not open file" << path;
            QMessageBox::warning(this, "Failed add file", QString("Can not open %1").arg(path));
            break;

        case TransportStatus::FILE_TOO_BIG:
            qWarning() << "Can not upload" << path << "file too big";
            break;

        case TransportStatus::GET_LAYOUT_FAILED:
            qCritical() << "Can not upload failed got UploadLayout";
            break;

        default:
            break;
        }
    }


}















