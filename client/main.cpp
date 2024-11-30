#include "mainwindow.h"

#include <QApplication>
#include <QLoggingCategory>

#include "widgetlogintitle.h"

#include "structs/foxcloudclientinfo.h"
#include "structs/webserverinfo.h"
#include "common/jsontool.h"
#include "common/config.h"

#include "login.h"
#include "common/logger.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    /* 为当前应用统一设置字体 */
    a.setFont(QFont("BM Jua", 14, QFont::Normal, false));

    /* 初始化 Logger */
    // QLoggingCategory::setFilterRules("*.debug=false;");  // 设置日志级别
    Logger::init(PATH_LOG, "hh:mm:ss", " | ");  // 初始化 Logger


    Login login;
    login.show();

    // MainWindow w;
    // w.show();

    // Logger::close();
    return a.exec();
}
