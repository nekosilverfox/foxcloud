#include "widgetlogintitle.h"
#include "ui_widgetlogintitle.h"

WidgetLoginTitle::WidgetLoginTitle(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WidgetLoginTitle)
{
    ui->setupUi(this);

    /* Init imgs */
    ui->lbLogo->setPixmap(QPixmap(":/img/foxcloud-logo.svg").scaled(QSize(80, 40), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    // ui->lbLogo->setScaledContents(true);
    ui->btnSetServer->setIcon(QIcon(QPixmap(":/img/setting-server.svg")));
    ui->btnHide->setIcon(QIcon(QPixmap(":/img/window-minimize.svg")));
    ui->btnClose->setIcon(QIcon(QPixmap(":/img/circle-xmark.svg")));


    /* 去边框 */
    this->setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    // this->setFont(QFont("BM Jua", 16, QFont::Bold, false));

    _parent = parent;
}

WidgetLoginTitle::~WidgetLoginTitle()
{
    delete ui;
}

/** 重写鼠标事件，允许按住鼠标左键拖动窗口
 * @brief WidgetLoginTitle::mouseMoveEvent
 * @param event
 */
void WidgetLoginTitle::mouseMoveEvent(QMouseEvent *event)
{
    /* 只允许左键按住拖动，持续的状态应该用 buttonS，并且用 `&` */
    if (event->buttons() & Qt::LeftButton)
    {
        /* globalPos 全局坐标，因为移动是基于整个屏幕的*/

        /* 窗口移动的点，并不是鼠标点击的点，而是当前窗口的左上角
         * ↓ 窗口所上角
         * ———————————————————————————————
         * |                             |
         * |                             |
         * |                             |
         * |                             |
         * ———————————————————————————————
         */
        _parent->move(event->globalPos() - _pt);
    }
}

void WidgetLoginTitle::mousePressEvent(QMouseEvent *event)
{
    // 鼠标按下的瞬间，瞬间的状态应该用 button，并且用 `==`
    if (event->button() == Qt::LeftButton)
    {
        _pt = event->globalPos() - _parent->geometry().topLeft(); // 差值：鼠标位置 - 窗口左上角
    }
}
