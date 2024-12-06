#ifndef DOWNLOADLAYOUT_H
#define DOWNLOADLAYOUT_H

#include <QWidget>
#include <QVBoxLayout>

#include "transportbar.h"


class DownloadLayout
{
public:
    static DownloadLayout* getInstance();

    void initDownloadLayout(QWidget* parent);
    bool appendTransportBar(TransportBar* bar);
    bool removeTransportBar(TransportBar* bar);

private:
    DownloadLayout();
    ~DownloadLayout();

private:
    static DownloadLayout* _instance;
    QWidget*     _widget;  // 窗口
    QVBoxLayout* _vlayout;  // _widget 的 垂直布局
};

#endif // DOWNLOADLAYOUT_H
