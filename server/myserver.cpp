#include "myserver.h"

myserver::myserver()
{

}

void myserver::startServer()
{
    if (this->listen(QHostAddress::Any,12345))
    {
        qDebug()<<"Server is ready";
    }
    else
    {
        qDebug()<<"Server is not ready";
    }
}

void myserver::incomingConnection(qintptr socketDescriptor)
{   
    socket = new QTcpSocket(this);
    socket->setSocketDescriptor(socketDescriptor);
    clientsList.append(socket);

    connect(socket, SIGNAL(disconnected()), this, SLOT(sockDisc()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(sockReady()));
    qDebug()<<"INFO: New connection with socket descriptor: " <<socketDescriptor;

    sendToClients(socket, "Successful connection to server", 1);
}

void myserver::sockReady()
{
    QTcpSocket* socket = (QTcpSocket*)sender();
    QDataStream input(socket);
    input.setVersion(QDataStream::Qt_DefaultCompiledVersion);

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

        qDebug()<<"FROM CLIENT: "<< str;
        //socket->write("FROM SERVER: "+ str);

        nextBlockSize = 0;
        sendToClients(socket, str, 0);
    }

}

void myserver::sockDisc()
{
    qDebug()<<"INFO: Lost connection with client";
}


void myserver::sendToClients(QTcpSocket* socket, const QString& str, int flag)
{
    QByteArray array;
    QDataStream sendStream(&array, QIODevice::WriteOnly);

    sendStream << quint16(0) << str;
    sendStream.device()->seek(0);
    sendStream << (quint16)(array.size() - sizeof(quint16));

    if (flag == 1)
    {
        socket->write(array);
    }
    else
    {
        for (auto &soc: clientsList)
        {
           soc->write(array);
        }
    }
}
