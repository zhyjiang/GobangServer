#include "networkserver.h"
#include <QDebug>
#include <QDataStream>
#include <QString>

NetworkServer::NetworkServer(QObject *parent):
    QObject(parent)
{

}

NetworkServer::~NetworkServer()
{
    delete listenSocket;
    delete readWriteSocket;
}

void NetworkServer::initServer()
{
    listenSocket = new QTcpServer;
    listenSocket->listen(QHostAddress::Any,8888);
    connect(listenSocket, SIGNAL(newConnection()), this, SLOT(acceptConnection()));
}

void NetworkServer::acceptConnection()
{
    readWriteSocket = listenSocket->nextPendingConnection();
    listenSocket->close();
    connect(readWriteSocket, SIGNAL(readyRead()), this, SLOT(recvMessage()));
}

void NetworkServer::recvMessage()
{
    QByteArray info;
    info.clear();
    info = readWriteSocket->readAll();
    QDataStream in(&info, QIODevice::ReadOnly);
    int temp;
    in >> temp;
    if(temp == 1)
    {
        int x, y, camp;
        in >> x >> y >> camp;
        emit setPieces(Step(x, y, camp));
    }
}

void NetworkServer::sendMessage(int state, Step step)
{
    QByteArray array;
    array.clear();
    switch (state)
    {
        case 1:
        {
            QByteArray datagram;
            datagram.clear();
            QDataStream out(&datagram,QIODevice::WriteOnly);
            out.setVersion(QDataStream::Qt_4_3);
            out << 1 << step.x << step.y << step.camp;
            readWriteSocket->write(datagram);
        }
    }
}

void NetworkServer::connectHost()
{
    this->readWriteSocket = new QTcpSocket;
    this->readWriteSocket->connectToHost(QHostAddress("59.66.131.143"),8888);
    connect(readWriteSocket,SIGNAL(readyRead()),this,SLOT(recvMessage()));
}
