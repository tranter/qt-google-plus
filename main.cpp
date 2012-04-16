#include <QtGui/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(resources);
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    w.startLoginDefault();

    return a.exec();
}
