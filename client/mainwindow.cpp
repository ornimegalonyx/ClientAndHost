#include "mainwindow.h"
#include "ui_mainwindow.h"

bool checkTheConnection = false;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    tcpSocket(new QTcpSocket(this))
{
    ui->setupUi(this);

    ui->lineEdit_2->setValidator(new QIntValidator(1, 99999, this));
    ui->connectButton->setDefault(true);
    ui->connectButton->setEnabled(false);
    ui->loginButton->setDefault(true);
    ui->loginButton->setEnabled(false);
    ui->disconnectButton->setDefault(true);
    ui->disconnectButton->setEnabled(false);
    in.setDevice(tcpSocket);
    in.setVersion(QDataStream::Qt_5_10);
    setWindowTitle(QGuiApplication::applicationDisplayName());

    connect(ui->lineEdit, SIGNAL(textChanged(QString)),
            this, SLOT(enableConnectButton()));
    connect(ui->lineEdit_2, SIGNAL(textChanged(QString)),
            this, SLOT(enableConnectButton()));
    connect(ui->lineEdit_3, SIGNAL(textChanged(QString)),
            this, SLOT(enableLoginButton()));
    connect(ui->lineEdit_4, SIGNAL(textChanged(QString)),
            this, SLOT(enableLoginButton()));
    connect(ui->connectButton, SIGNAL(clicked(bool)),
            this, SLOT(requestToConnect()));
    connect(ui->loginButton, SIGNAL(clicked(bool)),
            this, SLOT(slotSendToServer()));
    connect(ui->quitButton, SIGNAL(clicked(bool)),
            this, SLOT(close()));
    connect(ui->disconnectButton, SIGNAL(clicked(bool)),
            this, SLOT(slotDisconnectButton()));

    connect(tcpSocket, SIGNAL(connected()),
            this, SLOT(slotConnected()));
    connect(tcpSocket, SIGNAL(readyRead()),
            this, SLOT(slotReadyRead()));
    connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(slotError(QAbstractSocket::SocketError)));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::requestToConnect()
{
    ui->connectButton->setEnabled(false);
    tcpSocket->abort();
    tcpSocket->connectToHost(ui->lineEdit->text(),
                             ui->lineEdit_2->text().toInt());
}

void MainWindow::enableConnectButton()
{
    ui->connectButton->setEnabled(
                !ui->lineEdit->text().isEmpty() &&
                !ui->lineEdit_2->text().isEmpty()
                );
}

void MainWindow::slotReadyRead()
{
    in.startTransaction();
    in >> feedBack;

    if (!in.commitTransaction())
        return;

    ui->label_5->setText(feedBack);
}

void MainWindow::slotError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        QMessageBox::information(this, "Error", "You have been disconnected from the server.");
        ui->disconnectButton->setEnabled(false);
        ui->label_5->setText("Waiting for connection to the server.");
        checkTheConnection = false;
        enableLoginButton();
        break;
    case QAbstractSocket::HostNotFoundError:
        QMessageBox::information(this, "Error",
                                 tr("The host was not found. Please check the "
                                    "host name and port settings."));
        break;
    case QAbstractSocket::ConnectionRefusedError:
        QMessageBox::information(this, "Error",
                                 tr("The connection was refused by the peer. "
                                    "Make sure the fortune server is running, "
                                    "and check that the host name and port "
                                    "settings are correct."));
        break;
    default:
        QMessageBox::information(this, "Error",
                                 tr("The following error occurred: %1.")
                                 .arg(tcpSocket->errorString()));
    }

    ui->connectButton->setEnabled(true);
}

void MainWindow::slotSendToServer()
{
    QByteArray  arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_10);
    out /*<< quint16(0) */<< ui->lineEdit_3->text() + ", " + ui->lineEdit_4->text();

    //out.device()->seek(0);
    //out << quint16(arrBlock.size() - sizeof(quint16));

    tcpSocket->write(arrBlock);
    ui->lineEdit_3->setText("");
    ui->lineEdit_4->setText("");
}

void MainWindow::slotConnected()
{
  ui->disconnectButton->setEnabled(true);
  checkTheConnection = true;
  enableLoginButton();
}

void MainWindow::enableLoginButton()
{
    ui->loginButton->setEnabled(
                !ui->lineEdit_3->text().isEmpty() &&
                !ui->lineEdit_4->text().isEmpty() &&
                checkTheConnection
          );
}

void MainWindow::slotDisconnectButton()
{
  tcpSocket->abort();
  ui->label_5->setText("You just disconnected from the server.");
  ui->disconnectButton->setEnabled(false);
  ui->connectButton->setEnabled(true);
  checkTheConnection = false;
  enableLoginButton();
}
