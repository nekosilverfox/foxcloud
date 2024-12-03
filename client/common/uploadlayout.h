#ifndef UPLOADLAYOUT_H
#define UPLOADLAYOUT_H

#include <QVBoxLayout>

#include "transportbar.h"


/**
 * @brief 上传列表的布局（单例模式类）
 */
class UploadLayout
{
public:
    static UploadLayout* getInstance();

    void initUploadLayout(QWidget* parent);
    bool appendTransportBar(TransportBar* bar);
    bool removeTransportBar(TransportBar* bar);

private:
    UploadLayout();
    ~UploadLayout();

private:
    static UploadLayout* _instance;  // 唯一实例
    QWidget*     _widget;  // 窗口
    QVBoxLayout* _vlayout;  // _widget 的 垂直布局
};

#endif // UPLOADLAYOUT_H
