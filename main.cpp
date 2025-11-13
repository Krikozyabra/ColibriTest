#include "mainwindow.h"

#include <QApplication>
#include "config.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    Config *c = Config::GetInstace();
    w.show();
    return a.exec();
}
