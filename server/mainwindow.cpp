#include "mainwindow.h"
#include "ui_mainwindow.h"

int i = 0, numOfTrying = 1;
bool alreadyLogined = false;

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  setWindowTitle(QGuiApplication::applicationDisplayName());

  loginCheck << "Stepanov_Pavel, 123"
             << "Podusenko_Viktor, 371950"
             << "Admin, Admin";
  tcpServer = new QTcpServer(this);

  connect(tcpServer, SIGNAL(newConnection()),
          this,      SLOT(slotNewConnection()));

  if (!tcpServer->listen()){
    QMessageBox::critical(this, "Server error",
                          tr("Unable to start the server: %1.")
                          .arg(tcpServer->errorString()));
    close();
    return;
  }
  QString ipAddress;
  QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
  // use the first non-localhost IPv4 address
  for (int i = 0; i < ipAddressesList.size(); ++i) {
      if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
          ipAddressesList.at(i).toIPv4Address()) {
          ipAddress = ipAddressesList.at(i).toString();
          break;
      }
  }
  // if we did not find one, use IPv4 localhost
  if (ipAddress.isEmpty())
      ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
  ui->label->setText(tr("The server is running on\n\nIP: %1\nPort: %2\n\n")
                       .arg(ipAddress).arg(tcpServer->serverPort()));

}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::slotSendToClient(QTcpSocket* pSocket, const QString& str)
{
  QByteArray block;
  QDataStream out(&block, QIODevice::WriteOnly);
  out.setVersion(QDataStream::Qt_5_10);

  out << str;

  pSocket->write(block);
}

void MainWindow::slotReadFromClient()
{
  QTcpSocket* pClientSocket = (QTcpSocket*)sender();
  QDataStream in(pClientSocket);
  in.setVersion(QDataStream::Qt_5_10);

  QString str;
  in >> str;
  qDebug() << loginCheck << loginCheck.length();
  qDebug() << str;
  qDebug() << alreadyLogined;

  if(!alreadyLogined){
  for(i = 0; i < loginCheck.length(); i++)
    if(str == loginCheck.at(i)){
      switch (i) {
      case 0:
        slotSendToClient(pClientSocket, "Welcome Stepanov Pavel!");
        alreadyLogined = true;
        break;
      case 1:
        slotSendToClient(pClientSocket, "Welcome Podusenko Viktor!");
        alreadyLogined = true;
        break;
      case 2:
        slotSendToClient(pClientSocket, "Welcome Admin!");
        alreadyLogined = true;
        break;
      default:
        break;
      }
  break;
  }
  if(!alreadyLogined){
    if(numOfTrying == 3) {
      pClientSocket->disconnectFromHost();
      numOfTrying = 1;
    }
    if(numOfTrying < 3) {
      slotSendToClient(pClientSocket, tr("Access denied, number of tries left: %1") .arg(3-numOfTrying));
      numOfTrying++;
    }
  }
  } else slotSendToClient(pClientSocket, "You already connected, first disconnect.");
}

void MainWindow::slotNewConnection()
{
  QTcpSocket* pClientSocket = tcpServer->nextPendingConnection();
      connect(pClientSocket, SIGNAL(disconnected()),
              this,          SLOT(slotDefaultSettings()));
      connect(pClientSocket, SIGNAL(disconnected()),
              pClientSocket, SLOT(deleteLater()));
      connect(pClientSocket, SIGNAL(readyRead()),
              this,          SLOT(slotReadFromClient()));

      slotSendToClient(pClientSocket, "Server Response: Connected!");
}

void MainWindow::slotDefaultSettings()
{
  i = 0;
  numOfTrying = 1;
  alreadyLogined = false;
}

