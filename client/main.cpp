#include "mainwindow.h"

#include <QApplication>

#include "widgetlogintitle.h"

#include "structs/foxcloudclientinfo.h"
#include "structs/webserverinfo.h"
#include "common/jsontool.h"

#include "login.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    /* 为当前应用统一设置字体 */
    a.setFont(QFont("BM Jua", 14, QFont::Normal, false));

    Login login;
    login.show();

    // MainWindow w;
    // w.show();

    return a.exec();
}
