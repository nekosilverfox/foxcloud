#include <QApplication>
#include <QLoggingCategory>
#include <QFontDatabase>

#include "windowmanager.h"
#include "common/config.h"
#include "common/logger.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyleSheet(
        "QWidget { color: rgb(11, 54, 79); }"
        "QMessageBox { background-color: rgb(230, 240, 250); }"
        );
    a.setWindowIcon(QIcon(":/img/foxcloud-logo-name.svg"));

    /* 加载自定义字体文件 */
    int fontId = QFontDatabase::addApplicationFont(":/font/BM_JUA.ttf");
    if (fontId == -1)
    {
        qDebug() << "Failed to load custom font";
    }
    QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);
    qDebug() << "Set font family as" << fontFamily;

    QFont customFont(fontFamily, 14, QFont::Normal);
    a.setFont(customFont);

    /* 初始化 Logger */
    // QLoggingCategory::setFilterRules("*.debug=false;");  // 设置日志级别
    Logger::init(PATH_LOG, "hh:mm:ss", " | ");  // 初始化 Logger

    WindowManager::getManger().showLogin();

    return a.exec();
}
