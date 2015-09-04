#include "networkserver.h"
#include <QDebug>
#include <QDataStream>
#include <QNetworkInterface>
#include <QNetworkAddressEntry>
#include <QString>

NetworkServer::NetworkServer(QObject *parent):
    QObject(parent)
{
    readWriteSocket = new QTcpSocket(this);
    listenSocket = new QTcpServer(this);
    SudpSocket = new QUdpSocket(this);
    LudpSocket = new QUdpSocket(this);
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(broadcast()));
    m_timer.setInterval(1000);
}

NetworkServer::~NetworkServer()
{
    sendMessage(9, Step(0,0,0));
    readWriteSocket->close();
    listenSocket->close();
    SudpSocket->close();
    LudpSocket->close();
    delete readWriteSocket;
    delete SudpSocket;
    delete LudpSocket;
    delete listenSocket;
}

void NetworkServer::initServer()
{
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
    int temp = 8888;
    while(temp == 8888)
    {
        in >> temp;
        if(temp == 1)
        {
            int x, y, camp;
            in >> x >> y >> camp;
            emit setPieces(Step(x, y, camp));
        }
        else if(temp == 2)
        {
            in >> temp;
            emit changeCamp(temp);
        }
        else if(temp == 3)
        {
            in >> temp;
            emit recall(temp);
        }
        else if(temp == 4)
        {
            emit reStart();
        }
        else if(temp == 5)
        {
            int camp, wbNumw, wbNumb;
            in >> camp >> wbNumw >> wbNumb;
            emit changeState(camp, wbNumw, wbNumb);
        }
        else if(temp == 6)
        {
            emit askForRecall();
        }
        else if(temp == 7)
        {
            emit agreeRecall();
        }
        else if(temp == 8)
        {
            readWriteSocket->abort();
            emit isDisconnect();
        }
        in >> temp;
    }
}

void NetworkServer::sendMessage(int state, Step step)
{
    if(readWriteSocket->state() == QAbstractSocket::ConnectedState)
    {
        QByteArray array;
        array.clear();
        QDataStream out(&array,QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_3);
        switch (state)
        {
            case 1:
            {
                out << 1 << step.x << step.y << step.camp << 8888;
                readWriteSocket->write(array);
                break;
            }
            case 2:
            case 3:
            {
            qDebug() << state -1;
                out << 2 << state-1 << 8888;
                readWriteSocket->write(array);
                break;
            }
            case 4:
            {
                out << 3 << step.camp << 8888;
                readWriteSocket->write(array);
                break;
            }
            case 5:
            {
                out << 4 << 8888;
                readWriteSocket->write(array);
                break;
            }
            case 6:
            {
                out << 5 << step.x << step.y << step.camp << 8888;
                readWriteSocket->write(array);
                break;
            }
            case 7:
            {
                out << 6 << 8888;
                readWriteSocket->write(array);
                break;
            }
            case 8:
            {
                out << 7 << 8888;
                readWriteSocket->write(array);
                break;
            }
            case 9:
            {
                out << 8 << 8888;
                readWriteSocket->write(array);
                break;
            }
        }
    }
}

void NetworkServer::connectHost(QString ip)
{
    readWriteSocket->abort();
    readWriteSocket->connectToHost(QHostAddress(ip),8888);
    connect(readWriteSocket,SIGNAL(readyRead()),this,SLOT(recvMessage()));
}

void NetworkServer::getIP()
{
    QList<QNetworkInterface> list = QNetworkInterface::allInterfaces();
    foreach(QNetworkInterface temp, list)
    {
        QList<QNetworkAddressEntry> entryList = temp.addressEntries();
        foreach (QNetworkAddressEntry entry, entryList)
            if(!entry.ip().toString().contains("127.0.") && entry.ip().protocol() == QAbstractSocket::IPv4Protocol)
            {
                Sadress = entry.ip().toString();
                Badress = entry.broadcast().toString();
            }
    }
}

void NetworkServer::broadcast()
{
    QByteArray datagram;
    datagram.clear();
    datagram.append(m_name + " ");
    datagram.append(Sadress);
    qDebug() << datagram;
    SudpSocket->writeDatagram(datagram.data(), datagram.size(), QHostAddress(Badress), 5746);
}

void NetworkServer::listen()
{
    LudpSocket->bind(5746, QUdpSocket::ShareAddress);
    connect(LudpSocket, SIGNAL(readyRead()),this, SLOT(processPendingDatagrams()));
}

void NetworkServer::closeListen()
{
    LudpSocket->close();
}

void NetworkServer::closeWrite()
{
    m_timer.stop();
    SudpSocket->close();
}

void NetworkServer::processPendingDatagrams()
{
    QByteArray datagram;
    while (LudpSocket->hasPendingDatagrams())
    {
        datagram.resize(LudpSocket->pendingDatagramSize());
        LudpSocket->readDatagram(datagram.data(), datagram.size());
    }
    QString temp = datagram;
    emit findPlayer(temp);
}
