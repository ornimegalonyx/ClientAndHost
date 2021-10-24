#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDataStream>
#include <QTcpSocket>
#include <QDebug>
#include <QMessageBox>

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
    void requestToConnect();
    void enableConnectButton();
    void slotReadyRead();
    void slotError(QAbstractSocket::SocketError socketError);
    void slotSendToServer();
    void slotConnected();
    void enableLoginButton();
    void slotDisconnectButton();

private:
    Ui::MainWindow *ui;

    QTcpSocket *tcpSocket = nullptr;
    QDataStream in;
    QString feedBack;

};

#endif // MAINWINDOW_H
