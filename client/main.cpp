#include "mainwindow.h"

#include <QApplication>

#include "widgetlogintitle.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    WidgetLoginTitle wlt;
    wlt.show();

    // MainWindow w;
    // w.show();
    return a.exec();
}
