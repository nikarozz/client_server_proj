#ifndef MYSERVER_H
#define MYSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>


class myserver : public QTcpServer
{
    Q_OBJECT
private:
    QTcpSocket* socket;
    quint16 nextBlockSize = 0;
    QList <QTcpSocket*> clientsList;

public:
    myserver();
    QByteArray Data;
    QDataStream input;
public slots:
    void startServer();
    void incomingConnection(qintptr socketDescriptor);
    void sockReady();
    void sockDisc();
    void sendToClients(QTcpSocket* socket, const QString& str, int flag);

};

#endif // MYSERVER_H
