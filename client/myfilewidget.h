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
    explicit MyFileWidget(QWidget *parent = nullptr);
    ~MyFileWidget();

    void initListWidgetFiles();  // 初始化 lwFiles 文件列表
    void startCheckTransportQueue(size_t interval = 1000);

    /* 用于上传文件 */
    void selectUploadFilesAndAppendToQueue();
    void uploadFilesAction();
    void uploadRealFile(UploadFileInfo* file2Upload);

    /* 获取文件列表 */


private:
    Ui::MyFileWidget *ui;

    QTimer _transportChecker;  // 定时器，每隔一段时间检查上传任务队列或者下载队列
};

#endif // MYFILEWIDGET_H
