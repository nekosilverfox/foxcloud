#include "myfilewidget.h"
#include "ui_myfilewidget.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QHttpMultiPart>
#include <QHttpPart>
#include <QMimeDatabase>
#include <QClipboard>


#include "structs/fileinfo.h"
#include "structs/httpreplaycode.h"
#include "common/networktool.h"
#include "common/uploadqueue.h"
#include "common/downloadqueue.h"
#include "common/clientinfoinstance.h"
#include "common/jsontool.h"

MyFileWidget::MyFileWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MyFileWidget)
{
    ui->setupUi(this);

    _numberCloudFiles = 0;

    /* 初始化文件显示控件 */
    initListWidgetFiles();

    /* 开始定时触发定时器，以便检查上传/下载任务队列 */
    startCheckTransportQueue(1000);

    connect(ui->btnUpload, &QPushButton::clicked, this, &MyFileWidget::selectUploadFilesAndAppendToQueue);
    connect(ui->btnDownload, &QPushButton::clicked, this, &MyFileWidget::addSelectItemToDownloadQueue);
    connect(ui->btnShareURL, &QPushButton::clicked, this, &MyFileWidget::getShareURLForSelectItem);
    connect(ui->btnDelete, &QPushButton::clicked, this, &MyFileWidget::deleteFileActoin);
    connect(ui->btnRefresh, &QPushButton::clicked, this, [=](){
        getUserNumberFilesFromServer();
    });


    connect(this, &MyFileWidget::numberOfCloudFilesUpdated, this, [=](){
        qDebug() << "Catch signal MyFileWidget::numberOfCloudFilesUpdated, start get cloud file list";
        if (_numberCloudFiles > 0)
        {
            /* 一定要清空列表，不然会在第二次获取文件列表的时候造成奔溃 */
            clearCloudFileList();

            // 开始递归获取文件列表，这里是递归入口
            getUserFilesListFromServer(SortType::Normal, 0, 10);
        }
    });
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
    ui->lwFiles->setGridSize(QSize(80, 80));        //设置item大小

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
    qDebug() << "Check queues";
    connect(&_transportChecker, &QTimer::timeout, this, &MyFileWidget::uploadFilesAction);
    connect(&_transportChecker, &QTimer::timeout, this, &MyFileWidget::downloadFileAction);

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

/**
 * @brief MyFileWidget::uploadFilesAction 这里先确认 MD5，没有匹配到再调用方法启动真实上传
 */
void MyFileWidget::uploadFilesAction()
{
    UploadQueue* queue = UploadQueue::getInstance();
    /* 检查上传队列是否有任务 */
    if (queue->isQueueEmpty())
    {
        return;
    }

    /* 因为是单任务上传，所以查看队列里是否有任务在正在上传 */
    if (queue->isUploading())
    {
        return;
    }

    /* 准备 Request 用于检查当前文件 MD5 */
    ClientInfoInstance* clientInfo = ClientInfoInstance::getInstance();
    QString url = QString("http://%1:%2/md5").arg(clientInfo->getServerAddress(),
                                                  QString::number(clientInfo->getServerPort()));
    qDebug() << "Got URL:" << url;

    QNetworkRequest request;
    request.setUrl(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    /* 获取一个用于上传的任务，取出第0个上传任务，如果任务队列没有任务在上传，设置第0个任务上传 */
    UploadFileInfo* file2Upload = queue->getFileToUpload();
    QByteArray postData = JsonTool::getCheckMD5JsonForServer(clientInfo->getLogin(),
                                                             clientInfo->getToken(),
                                                             file2Upload->name,
                                                             file2Upload->md5);
    QNetworkReply* reply = NetworkTool::getNetworkManager()->post(request, postData);
    connect(reply,  &QNetworkReply::finished, this, [=](){
        if (reply->error() != QNetworkReply::NoError)
        {
            qCritical() << "Check md5 from server failed" << reply->errorString();
            reply->deleteLater();
            return;
        }
        QByteArray replyData = reply->readAll();
        reply->deleteLater();

        const QString code = NetworkTool::getReplayCode(replyData);
        if (code == HttpReplayCode::CheckMD5::FILE_EXIST)
        {
            qInfo() << file2Upload->name << "exist on server";
            file2Upload->isUploaded = true;  // 设置一下标志位，以便队列识别和移除
            queue->removeFinsishedTask();
        }
        else if (code == HttpReplayCode::CheckMD5::SUCCESS)
        {
            qInfo() << file2Upload->name << "successful upload to server";
            file2Upload->isUploaded = true;
            queue->removeFinsishedTask();
        }
        else if (code == HttpReplayCode::CheckMD5::FAIL)  // 秒传失败，需要启用真正上传
        {
            qInfo() << file2Upload->name << "match MD5 failed, need to upload";
            uploadRealFile(file2Upload);
        }
        else if (code == HttpReplayCode::CheckMD5::TOKEN_ERROR)
        {
            qWarning() << "Failed token authentication";
            QMessageBox::warning(this, "Account Exception", "Please log in again");

            //TODO 发送重新登陆信号
        }
    });

}


/**
 * @brief MyFileWidget::uploadRealFile 开始上传真实文件
 * @param file2Upload 需要上传的文件
 */
void MyFileWidget::uploadRealFile(UploadFileInfo* file2Upload)
{
    qInfo() << "Start upload real file" << file2Upload->name;

    /*
       ------WebKitFormBoundary88asdgewtgewx\r\n
       Content-Disposition: form-data; user="mike"; filename="xxx.jpg"; md5="xxxx"; size=10240\r\n
       Content-Type: application/octet-stream\r\n
       \r\n
       真正的文件内容\r\n
       ------WebKitFormBoundary88asdgewtgewx
       */

    /* Content-Disposition */
    ClientInfoInstance* client = ClientInfoInstance::getInstance();
    QHttpPart part;
    QString disp = QString("form-data; "
                           "user=\"%1\"; "
                           "filename=\"%2\"; "
                           "md5=\"%3\"; "
                           "size=%4")
                       .arg(client->getLogin(),
                            file2Upload->name,
                            file2Upload->md5,
                            QString::number(file2Upload->size));
    qDebug() << "Content-Disposition:" << disp;
    part.setHeader(QNetworkRequest::ContentDispositionHeader, disp);

    /* Content-Type */
#if 1
    // 动态设置Content-Type，服务器端不支持，千万别用 qwq
    QMimeDatabase mimeDatabase;
    QMimeType mimeType = mimeDatabase.mimeTypeForFile(file2Upload->name);
    QString mimeName = mimeType.isValid() ? mimeType.name() : "application/octet-stream";
    part.setHeader(QNetworkRequest::ContentTypeHeader, mimeName);
#else
    part.setHeader(QNetworkRequest::ContentTypeHeader, "application/octet-stream");   /// Qt 6.8.2 有错误！！！不要使用 6.8.2 版本！！！！！
#endif
    part.setBodyDevice(file2Upload->pfile);

    QHttpMultiPart* multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    multiPart->append(part);

    QNetworkRequest request;
    QString url = QString("http://%1:%2/upload")
                      .arg(client->getServerAddress(),
                           QString::number(client->getServerPort()));
    request.setUrl(url);
    qDebug() << "Got URL:" << url;

    // 注意：`QHttpMultiPart` 会自动设置适当的 `Content-Type`，不需要手动设置
    // 如果手动设置，可能会覆盖自动生成的边界信息，导致请求失败
    // 所以这里不需要设置 `Content-Type`，让 `QHttpMultiPart` 处理
    // request.setHeader(QNetworkRequest::ContentTypeHeader, "multipart/form-data");
    QNetworkReply* reply = NetworkTool::getNetworkManager()->post(request, multiPart);

    // 确保在合适的时候删除 `multiPart`，例如在 `reply` 的 `finished` 信号中
    multiPart->setParent(reply); // 让 `multiPart` 在 `reply` 结束时自动删除

    /* 跳转到上传界面 */
    emit MyFileWidget::jumpToTabUpload();

    /*
     * 参数说明：
     *      bytesSent：已经上传的字节数。
     *      bytesTotal：要上传的总字节数。如果无法确定总字节数，这个值可能是 -1。
     *      信号触发：每当上传过程中有新的数据被发送到服务器时，这个信号会被触发。
     */
    connect(reply, &QNetworkReply::uploadProgress, this, [=](qint64 bytesSent, qint64 bytesTotal){
        if (bytesTotal > 0)
        {
            file2Upload->bar->setValue(bytesSent);
        }
    });

    /* 所有数据发送完成 */
    UploadQueue* queue = UploadQueue::getInstance();
    connect(reply, &QNetworkReply::finished, this, [=](){
        QString curFileName = file2Upload->name;
        qDebug() << "Reply upload file" << curFileName << "finished";

        /* 无论如何，设置完成 */
        file2Upload->isUploaded = true;
        queue->removeFinsishedTask();

        if (reply->error() != QNetworkReply::NoError)
        {
            QString err = "Upload file" + curFileName + "failed" + reply->errorString();
            qCritical() << err;
            QMessageBox::warning(this, "Something wrong", err);
            reply->deleteLater();
            return;
        }

        QByteArray replyData = reply->readAll();
        reply->deleteLater();
        multiPart->deleteLater();

        const QString code = NetworkTool::getReplayCode(replyData);
        if (code == HttpReplayCode::Upload::SUCCESS)
        {
            qInfo() << curFileName << "success upload to server";
        }
        else if (code == HttpReplayCode::Upload::FAIL)
        {
            qCritical() << curFileName << "FAILED upload to server";
        }
        else
        {
            qCritical() << "Unknow reply code" << code;
        }
    });

}

/**
 * @brief MyFileWidget::addSelectItemToDownloadQueue 将选中的文件添加到任务队列
 */
void MyFileWidget::addSelectItemToDownloadQueue()
{
    /* 获取用户选中项 */
    QListWidgetItem* item = ui->lwFiles->currentItem();
    if (nullptr == item)
    {
        qInfo() << "Not select any item";
        return;
    }

    DownloadQueue* queue = DownloadQueue::getInstance();
    if (nullptr == queue)
    {
        qCritical() << "Download queue is nullptr, download stop";
        return;
    }

    /* 根据用户选中的项去查找在本地存储的 云端文件信息 */
    CloudFileInfo* cloudFileToDownload = findItemInCloudFileList(item);
    if (nullptr == cloudFileToDownload)
    {
        qCritical() << "Can not find this item in local cloudfilelist";
        QMessageBox::warning(this, "Warning", "Can not find this file, please reflash the file list");
        return;
    }


    /* 选择并拼接一个保存路径，之后追加到队列 */
    QString savePath = QFileDialog::getSaveFileName(this, "Save to", QString("%1%2%3").arg(QDir::homePath(), QDir::separator(), cloudFileToDownload->fileName));
    if (savePath.isEmpty())
    {
        qInfo() << "User cancel download file";
        return;
    }
    qInfo() << cloudFileToDownload->fileName << "will save to" << savePath;

    TransportStatus status = queue->appendTaskToQueue(cloudFileToDownload, savePath);
    switch (status)
    {
    case TransportStatus::ALREADY_IN_QUEUE:
        QMessageBox::warning(this, "warning", QString("%1 aleady in download queue").arg(cloudFileToDownload->fileName));
        break;

    case TransportStatus::OPEN_FAILED:
        qCritical() << "Can not open" << savePath;
        break;

    default:
        break;
    }

    emit jumpToTabDownload();
}

void MyFileWidget::downloadFileAction()
{
    DownloadQueue* queue = DownloadQueue::getInstance();
    if (nullptr == queue)
    {
        qCritical() << "Download queue is nullptr";
        return;
    }

    /// TODO 多线程这里要改掉
    if (queue->isQueueEmpty() || queue->isDownloading())
    {
        // qDebug() << "Download queue is empty" << queue->isQueueEmpty() << "or task is downloading";
        return;
    }

    DownloadFileInfo* task = queue->getFileToDownload();
    task->isDownloading = true;

    /* http://172.17.0.2:80/group1/M00/00/00/xxxxxxxxxxxxxxxxxxxx.png
     * 需要截取从 "/group" 到最后的内容，然后再拼接服务器 IP:PORT
     */
    QString url = task->url;
    QString startMarker = "/group";
    int startIndex = url.indexOf(startMarker);  // 找到起始位置
    if (startIndex != -1)
    {
        url = url.mid(startIndex);  // 提取从 "/group" 开始的内容: "/group1/M00/00/00/xxxxxxxxxxxxxxxxxxxx.png"
    }
    ClientInfoInstance* client = ClientInfoInstance::getInstance();
    url = QString("http://%1:%2%3").arg(client->getServerAddress(), QString::number(client->getServerPort()), url);

    QNetworkRequest request(url);
    QNetworkReply* reply = NetworkTool::getNetworkManager()->get(request);
    qInfo() << "Start download from URL" << url;

    if (nullptr == reply)
    {
        task->isDownloaded = true;
        queue->removeFinsishedTask();
        qCritical() << "Download error, reply is nullptr";
        return;
    }

    /* 有可用数据时候 */
    connect(reply, &QNetworkReply::readyRead, this, [=](){
        if (nullptr != task->pfile)
        {
            task->pfile->write(reply->readAll());
        }
    });

    /* 进度更新的时候 */
    connect(reply, &QNetworkReply::downloadProgress, this, [=](qint64 bytesRead, qint64 bytesTotal){
        task->bar->setValue(bytesRead);
    });

    /* 请求完成时 */
    connect(reply, &QNetworkReply::finished, this, [=](){
        qInfo() << task->fileName << "download finished";
        reply->deleteLater();
        task->isDownloaded = true;
        queue->removeFinsishedTask();
    });
}


/**
 * @brief MyFileWidget::deleteFileActoin 从服务器中删除文件
 */
void MyFileWidget::deleteFileActoin()
{
    qInfo() << "Start to delete file from cloud";

    QListWidgetItem* curItem = ui->lwFiles->currentItem();  // 如果什么都选中，则为空指针
    if (nullptr == curItem)
    {
        QMessageBox::warning(this, "Warning", "Do not select any item");
        qInfo() << "Do not select any item to delete, delte cancal";
        return;
    }

    CloudFileInfo* file = findItemInCloudFileList(curItem);
    if (nullptr == file)
    {
        QMessageBox::warning(this, "Error", "Do not find this item in cloud file list, please refrish the list");
        qCritical() << "Do not find select CloudFileInfo in cloud file list, delete cancal";
        return;
    }


    QMessageBox::StandardButton btn = QMessageBox::question(this, "Delete file",
                                                            QString("Are you sure to delete %1?").arg(file->fileName),
                                                            QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if (QMessageBox::No == btn)
    {
        qInfo() << "Delete" << file->fileName << "cancel";
        return;
    }


    ClientInfoInstance* client = ClientInfoInstance::getInstance();
    QString url = QString("http://%1:%2/dealfile?cmd=del")
                      .arg(client->getServerAddress(), QString::number(client->getServerPort()));
    qDebug() << "Url for delete file from cloud:" << url;

    QNetworkRequest request;
    request.setUrl(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");

    QByteArray data = JsonTool::getDelteFileJsonFromServer(client->getLogin(), client->getToken(), file->fileName, file->md5);
    QNetworkReply * reply = NetworkTool::getNetworkManager()->post(request, data);

    connect(reply, &QNetworkReply::finished, this, [=](){
        if (reply->error() != QNetworkReply::NoError)
        {
            qCritical() << "Delete file from server failed" << reply->errorString();
            reply->deleteLater();
            return;
        }

        QByteArray replyData = reply->readAll();
        reply->deleteLater();
        qDebug() << "Got reply from server" << replyData;

        const QString code = NetworkTool::getReplayCode(replyData);
        if (HttpReplayCode::DeleteFile::SUCCESS == code)
        {
            qInfo() << "Successful delete file from server";

            /* 通过发送刷新按钮被点击来刷新列表 */
            emit ui->btnRefresh->clicked();
        }
        else if (HttpReplayCode::DeleteFile::TOKEN_ERROR == code)
        {
            qWarning() << "Failed token authentication";
            QMessageBox::warning(this, "Account Exception", "Please log in again");
            // TODO 发送重新登陆信号
            return;
        }
        else if (HttpReplayCode::DeleteFile::FAIL == code)
        {
            qCritical() << "Failed delete file from server";
            QMessageBox::warning(this, "Failed", "Failed delete file from server");
        }
        else
        {
            qCritical() << "Unknow reply code" << code;
        }

    });
}

/**
 * @brief MyFileWidget::clearListWidgetFiles 清空 lwFiles 中所有的 Item
 */
void MyFileWidget::clearListWidgetFiles()
{
    ui->lwFiles->clear();

    qInfo() << "Clear all items in lwFiles";
}

/**
 * @brief MyFileWidget::refreshListWidgetFiles 刷新lwFile中的内容
 */
void MyFileWidget::refreshListWidgetFiles()
{
    qInfo() << "Start refrsh lwFiles";
    clearListWidgetFiles();

    if (_cloudFileList.isEmpty())
    {
        return;
    }

    for (CloudFileInfo* i : _cloudFileList)
    {
        ui->lwFiles->addItem(i->item);
    }
    qInfo() << "Finished add all item in cloud file list";
}

/**
 * @brief MyFileWidget::getUserNumberFilesFromServer 从服务端获取用户文件数量
 */
void MyFileWidget::getUserNumberFilesFromServer()
{
    qInfo() << "Start get number of cloud files from server";

    ClientInfoInstance* client = ClientInfoInstance::getInstance();
    QString url = QString("http://%1:%2/myfiles?cmd=count")
                      .arg(client->getServerAddress(), QString::number(client->getServerPort()));
    qDebug() << "Url for get number of cloud files from server:" << url;

    QNetworkRequest request;
    request.setUrl(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");

    QByteArray postData = JsonTool::getUserNumberFilesJsonForServer(client->getLogin(), client->getToken());
    QNetworkReply* reply = NetworkTool::getNetworkManager()->post(request, postData);

    connect(reply, &QNetworkReply::finished, this, [=](){
        if (reply->error() != QNetworkReply::NoError)
        {
            qCritical() << "Got number of user cloud files from server failed" << reply->errorString();
            reply->deleteLater();
            return;
        }

        QByteArray replyData = reply->readAll();
        reply->deleteLater();
        qDebug() << "Got reply from server" << replyData;

        const QString code = NetworkTool::getReplayCode(replyData);
        if (HttpReplayCode::UserNumberCloudFiles::SUCCESS == code)
        {
            qInfo() << "Successful got number of user cloud files from server";
        }
        else if (HttpReplayCode::UserNumberCloudFiles::FAIL == code)
        {
            qWarning() << "Failed token authentication";
            QMessageBox::warning(this, "Account Exception", "Please log in again");
            // TODO 发送重新登陆信号
            return;
        }
        else
        {
            qCritical() << "Unknow reply code" << code;
            // return;
        }

        size_t numCloudFiles = NetworkTool::getReplayNumberFiles(replyData);
        qInfo() << "Got number of user cloud files from server:" << numCloudFiles;

        _numberCloudFiles = numCloudFiles;
        qInfo() << "Updated number of user cloud files to" << _numberCloudFiles;

        /* 发送信号去获取用户文件列表 */
        emit MyFileWidget::numberOfCloudFilesUpdated();
    });
}

/**
 * @brief MyFileWidget::getUserFilesListFromServer
 * @param softType 文件的排序方式
 * @param startPos 本次请求开始的位置（从数据表中的哪个位置开始取）
 * @param numPerRequest 每次请求的数量（从数据表中的那个位置取多少个）
 */
void MyFileWidget::getUserFilesListFromServer(const SortType softType, const size_t startPos, const size_t numPerRequest)
{
    qInfo() << "Start get file list from server, need to get" << _numberCloudFiles
            << "will start with postion" << startPos
            << "get" << numPerRequest << "items this time";

    size_t curStartPos = startPos;
    size_t curRequestNum = numPerRequest;  // 本次要请求的数量

    // 遍历数目，结束条件处理
    if (_numberCloudFiles <= 0)  // 结束条件，这个条件很重要，函数递归的结束条件
    {
        qInfo() << "Finish got user cloud file list";
        refreshListWidgetFiles();
        return;
    }
    else if (numPerRequest > _numberCloudFiles)
    {
        curRequestNum = _numberCloudFiles;
    }


    // 获取用户文件信息 127.0.0.1:80/myfiles&cmd=normal
    // 按下载量升序 127.0.0.1:80/myfiles?cmd=pvasc
    // 按下载量降序127.0.0.1:80/myfiles?cmd=pvdesc
    QString cmd;
    switch (softType)
    {
    case SortType::Normal:
        cmd = JsonKeyForServer::UserFilesList::STR_SORT_TYPE_NORMAL;
        break;

    case SortType::AscDownloadCount:
        cmd = JsonKeyForServer::UserFilesList::STR_SORT_TYPE_ASC;
        break;

        case SortType::DescDownloadCount:
        cmd = JsonKeyForServer::UserFilesList::STR_SORT_TYPE_DESC;
        break;

    default:
        cmd = JsonKeyForServer::UserFilesList::STR_SORT_TYPE_NORMAL;
        break;
    }

    ClientInfoInstance* client = ClientInfoInstance::getInstance();
    QString url = QString("http://%1:%2/myfiles?cmd=%3")
                      .arg(client->getServerAddress(), QString::number(client->getServerPort()), cmd);
    qDebug() << "Url for get file list from server:" << url;

    QNetworkRequest request;
    request.setUrl(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");

    QByteArray postData = JsonTool::getUserFilesListJsonForServer(client->getLogin(), client->getToken(),
                                                                  curStartPos, numPerRequest);
    /* 改变下次请求位置 */
    curStartPos += numPerRequest;
    _numberCloudFiles -= numPerRequest;

    QNetworkReply* reply = NetworkTool::getNetworkManager()->post(request, postData);
    connect(reply, &QNetworkReply::finished, this, [=](){
        if (reply->error() != QNetworkReply::NoError)
        {
            qCritical() << "Got file list from server failed" << reply->errorString();
            reply->deleteLater();
            return;
        }

        QByteArray replyData = reply->readAll();
        reply->deleteLater();
        qDebug() << "Got reply from server" << replyData;

        const QString code = NetworkTool::getReplayCode(replyData);
        if (HttpReplayCode::GetUserFilesList::FAIL == code)
        {
            qCritical() << "Got file list from server failed";
            return;
        }
        else if (HttpReplayCode::GetUserFilesList::TOKEN_ERROR == code)
        {
            qWarning() << "Failed token authentication";
            QMessageBox::warning(this, "Account Exception", "Please log in again");
            // TODO 发送重新登陆信号
            return;
        }

        /* 如果没错误就递归获取 */
        _cloudFileList.append(NetworkTool::getReplayCloudFilesList(replyData));
        getUserFilesListFromServer(softType, curStartPos, numPerRequest);
    });
}

/**
 * @brief MyFileWidget::getShareURLForSelectItem 获取选中项的分享 URL （需要在服务端设置 Nginx 允许访问）
 * @return 分享 URL
 */
QString MyFileWidget::getShareURLForSelectItem()
{
    QListWidgetItem* curItem = ui->lwFiles->currentItem();  // 如果什么都选中，则为空指针
    if (nullptr == curItem)
    {
        QMessageBox::warning(this, "Warning", "Do not select any item");
        qInfo() << "Do not select any item";
        return "";
    }

    CloudFileInfo* file = findItemInCloudFileList(curItem);
    if (nullptr == file)
    {
        QMessageBox::warning(this, "Warning", "Do not find this item in cloud file list");
        qCritical() << "Do not find this CloudFileInfo in cloud file list";
        return "";
    }

    /* http://172.17.0.2:80/group1/M00/00/00/xxxxxxxxxxxxxxxxxxxx.png
     * 需要截取从 "/group" 到最后的内容，然后再拼接服务器 IP:PORT
     */
    QString url = file->url;
    QString startMarker = "/group";
    int startIndex = url.indexOf(startMarker);  // 找到起始位置
    if (startIndex != -1)
    {
        url = url.mid(startIndex);  // 提取从 "/group" 开始的内容: "/group1/M00/00/00/xxxxxxxxxxxxxxxxxxxx.png"
    }
    else
    {
        qCritical() << "Marker not found!";
        return file->url;
    }
    ClientInfoInstance* client = ClientInfoInstance::getInstance();
    url = QString("http://%1:%2%3").arg(client->getServerAddress(), QString::number(client->getServerPort()), url);

    /* 创建提示拷贝 URL 消息框 */
    QMessageBox msgBox;
    msgBox.setWindowTitle("Share URL");
    msgBox.setText(url);
    msgBox.setIcon(QMessageBox::Information);
    msgBox.addButton("Copy", QMessageBox::ActionRole);  // 添加自定义“复制”按钮
    msgBox.exec();

    /* 将 URL 复制到剪贴板 */
    QClipboard* clipboard = QGuiApplication::clipboard();
    clipboard->setText(url);

    // QMessageBox::information(this, "Share URL", url);
    qDebug() << "Get share URL:" << url;

    return url;
}

/**
 * @brief MyFileWidget::findItemInCloudFileList 通过 QListWidgetItem（可以是用户选中的项） 在 cloudFileList 中查找文件对象，并获取这个对象，之后用于下载文件或者分享链接
 * @param item QListWidgetItem
 * @return 对应的 CloudFileInfo 对象；如果未找到，返回 nullptr
 */
CloudFileInfo* MyFileWidget::findItemInCloudFileList(const QListWidgetItem* item)
{
    for (CloudFileInfo* i : _cloudFileList)
    {
        if (item == i->item)
        {
            return i;
        }
    }

    return nullptr;
}

/**
 * @brief MyFileWidget::clearCloudFileList 清空存储云端文件信息的列表
 */
void MyFileWidget::clearCloudFileList()
{
    while (_cloudFileList.size() != 0)
    {
        CloudFileInfo* cur = _cloudFileList.takeFirst();

        delete cur->item;
        cur->item = nullptr;

        delete cur;
        cur = nullptr;
    }
}

