#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setApplicationDisplayName("Client");
    MainWindow w;
    w.move(800,450);
    w.show();

    return a.exec();
}
