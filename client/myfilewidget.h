#ifndef MYFILEWIDGET_H
#define MYFILEWIDGET_H

#include <QWidget>
#include <QTimer>

#include "structs/fileinfo.h"

namespace Ui {
class MyFileWidget;
}

class MyFileWidget : public QWidget
{
    Q_OBJECT

public:
    /* 排序方式：正常、按下载量升序、按下载量降序 */
    enum SortType {Normal, AscDownloadCount, DescDownloadCount};

public:
    explicit MyFileWidget(QWidget *parent = nullptr);
    ~MyFileWidget();

    void initListWidgetFiles();  // 初始化 lwFiles 文件列表
    void startCheckTransportQueue(size_t interval = 1000);

    /* 用于上传文件 */
    void selectUploadFilesAndAppendToQueue();
    void uploadFilesAction();
    void uploadRealFile(UploadFileInfo* file2Upload);

    /* 当前 Widget 的显示相关 */
    void clearListWidgetFiles();
    void refreshListWidgetFiles();

    /* 获取文件列表 */
    void getUserNumberFilesFromServer();
    void getUserFilesListFromServer(const SortType softType);


    /* 操作私有成员 */
    void clearCloudFileList();


private:
    Ui::MyFileWidget *ui;

    QList<CloudFileInfo*> _cloudFileList;  // 用户云端文件列表

    QTimer _transportChecker;  // 定时器，每隔一段时间检查上传任务队列或者下载队列
};

#endif // MYFILEWIDGET_H
