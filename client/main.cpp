#include <QApplication>
#include <QLoggingCategory>

#include "windowmanager.h"
#include "common/config.h"
#include "common/logger.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setFont(QFont(":/font/BM_JUA.ttf", 14, QFont::Normal, false));
    a.setStyleSheet(
        "QWidget { color: rgb(11, 54, 79); }"
        "QMessageBox { background-color: rgb(230, 240, 250); }"
        );
    a.setWindowIcon(QIcon(":/img/foxcloud-logo-name.svg"));

    /* 初始化 Logger */
    // QLoggingCategory::setFilterRules("*.debug=false;");  // 设置日志级别
    Logger::init(PATH_LOG, "hh:mm:ss", " | ");  // 初始化 Logger

    WindowManager::getManger().showLogin();

    return a.exec();
}
