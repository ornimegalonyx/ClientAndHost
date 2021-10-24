#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  QApplication::setApplicationDisplayName("Server");
  MainWindow w;
  w.move(600, 450);
  w.show();

  return a.exec();
}
