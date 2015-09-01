#ifndef NETWORKSERVER_H
#define NETWORKSERVER_H

#include "step.h"

#include <QObject>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>

class Gobang;

class NetworkServer : public QObject
{
    Q_OBJECT

public:
    explicit NetworkServer(QObject *parent = 0);
    ~NetworkServer();

signals:
    void setPieces(Step);

private slots:
    void initServer();
    void acceptConnection();
    void recvMessage();
    void sendMessage(int state, Step step);
    void connectHost();

private:
    QTcpServer  *listenSocket;
    QTcpSocket  *readWriteSocket;
    friend class Gobang;
};

#endif // NETWORKSERVER_H
