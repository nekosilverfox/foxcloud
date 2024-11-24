#include "mainwindow.h"

#include <QApplication>

#include "widgetlogintitle.h"
#include "login.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    /* 为当前应用统一设置字体 */
    a.setFont(QFont("BM Jua", 14, QFont::Normal, false));

    // WidgetLoginTitle wlt;
    // wlt.show();

    Login login;
    login.show();

    // MainWindow w;
    // w.show();
    return a.exec();
}
