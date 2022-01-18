#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    socket = new QTcpSocket(this);

    connect(socket, SIGNAL(readyRead()), this, SLOT(sockReadyRead()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(sockDisc()));
    connect(socket, SIGNAL(clicked()), this, SLOT(on_sendButton_clicked()));

    ui->historyWindow->setReadOnly(true);
}

quint16 MainWindow::port()
{
    QString str = ui->portLineEdit->text();
    QTextStream qts(&str);
    quint16 port1 = 0;
    qts >> port1;
    return port1;
}

void MainWindow::on_pushButton_clicked()
{
    socket->connectToHost(ui->ipLineEdit->text(), MainWindow::port());
}

void MainWindow::sockReadyRead()
{
    QDataStream input(socket);
    for(;;)
    {
        if (!nextBlockSize)
        {
            if (socket->bytesAvailable() < sizeof(quint16))
            {
                break;
            }
            input >> nextBlockSize;
        }

        if (socket->bytesAvailable() < nextBlockSize)
        {
            break;
        }
        QString str;
        input >> str;

        ui->historyWindow->append("FROM SERVER: " + str);
        nextBlockSize = 0;
    }
}

void MainWindow::sockDisc()
{
    ui->historyWindow->append("INFO: Lost connection with server");
}

void MainWindow::on_sendButton_clicked()
{
    QByteArray array;
    QDataStream sendStream(&array, QIODevice::WriteOnly);

    sendStream << quint16(0) << ui ->lineEdit->text();
    sendStream.device()->seek(0);
    sendStream << (quint16)(array.size() - sizeof(quint16));

    socket->write(array);
    ui ->lineEdit->setText("");
}
