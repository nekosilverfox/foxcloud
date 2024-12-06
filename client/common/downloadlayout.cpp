#include "downloadlayout.h"

DownloadLayout* DownloadLayout::_instance = new DownloadLayout;

DownloadLayout::DownloadLayout()
{
    _widget  = nullptr;
    _vlayout = nullptr;
}


DownloadLayout *DownloadLayout::getInstance()
{
    return _instance;
}

/**
 * @brief DownloadLayout::initDownloadLayout 初始化一个 Download Layout
 * @param parent
 */
void DownloadLayout::initDownloadLayout(QWidget* parent)
{
    qInfo() << "Init Download Layout";

    /* 初始化当前 widget */
    _widget = new QWidget(parent);

    /* 为当前窗口设置一个垂直 Layout */
    _vlayout = new QVBoxLayout;
    _widget->setLayout(_vlayout);
    _vlayout->setContentsMargins(0, 0, 0, 0);   // 设置边缘空隙
    _vlayout->addStretch();                     // 添加一弹簧


    /* 当前 _widget 添加到父 widget */
    QLayout* parentLayout = parent->layout();  // 获取父窗口布局
    parentLayout->addWidget(_widget);          // 把当前 widget 添加到父窗口布局中
    parentLayout->setContentsMargins(0, 0, 0, 0);   // 设置父窗口边缘空隙

    return;
}

/**
 * @brief DownloadLayout::appendTransportBar 向传输列表 Layout 最后添加一个 TransportBar
 * @param bar
 * @return
 */
bool DownloadLayout::appendTransportBar(TransportBar *bar)
{
    if (nullptr == _vlayout || nullptr == _widget)
    {
        qCritical() << "Download Layout is nullptr, can not insert bar";
        return false;
    }

    if (nullptr == bar)
    {
        qWarning() << "TransportBar is nullprt can not insert to Upload Layout";
        return false;
    }

    _vlayout->insertWidget(_vlayout->count() - 1, bar);  // 最后一个是弹簧，所以要 -1
    qInfo() << "Add new TransportBar to DownloadLayout";
    return true;
}

/**
 * @brief DownloadLayout::removeTransportBar 从传输列表中移除 TransportBar
 * @param bar 要移除的自定义的 TransportBar
 * @return 是否成功
 */
bool DownloadLayout::removeTransportBar(TransportBar *bar)
{
    if (nullptr == _vlayout || nullptr == _widget)
    {
        qCritical() << "Download Layout is nullptr, remove bar failed";
        return false;
    }

    if (nullptr == bar)
    {
        qWarning() << "TransportBar is nullprt can not remove from Download Layout";
        return false;
    }

    _vlayout->removeWidget(bar);

    qInfo() << "Remove TransportBar from DownloadLayout";
    return true;
}

