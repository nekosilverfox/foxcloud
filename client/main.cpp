#include "mainwindow.h"

#include <QApplication>
#include <QLoggingCategory>

#include "windowmanager.h"
#include "common/config.h"
#include "common/logger.h"

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    /* 为当前应用统一设置字体 */
    a.setStyleSheet("QWidget { color: rgb(11, 54, 79); }");
    a.setFont(QFont("BM Jua", 14, QFont::Normal, false));
    a.setWindowIcon(QIcon(":/img/foxcloud-logo-name.svg"));

    /* 初始化 Logger */
    // QLoggingCategory::setFilterRules("*.debug=false;");  // 设置日志级别
    Logger::init(PATH_LOG, "hh:mm:ss", " | ");  // 初始化 Logger

    // WindowManager::getManger().showLogin();

    MainWindow m;
    m.show();

    return a.exec();
}
