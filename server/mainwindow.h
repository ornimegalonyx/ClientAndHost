#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QTcpServer>
#include <QDebug>
#include <QList>
#include <QMessageBox>
#include <QNetworkInterface>
#include <QTcpSocket>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();
private slots:
  void slotSendToClient(QTcpSocket* pSocket, const QString& str);
  void slotReadFromClient();
  void slotNewConnection();
  void slotDefaultSettings();

private:
  Ui::MainWindow *ui;
  QTcpServer *tcpServer = nullptr;
  QList<QString> loginCheck;

};

#endif // MAINWINDOW_H
