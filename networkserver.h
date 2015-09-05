#ifndef NETWORKSERVER_H
#define NETWORKSERVER_H

#include "step.h"

#include <QObject>
#include <QString>
#include <QTimer>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QUdpSocket>
#include <QtNetwork/QHostAddress>

class MainWindow;

class NetworkServer : public QObject
{
    Q_OBJECT

public:
    explicit NetworkServer(QObject *parent = 0);
    ~NetworkServer();

signals:
    void setPieces(Step);
    void findPlayer(QString);
    void changeCamp(int);
    void recall(int);
    void reStart();
    void changeState(int, int, int);
    void askForRecall();
    void agreeRecall();
    void agreeExit();
    void askForExit();
    void refuse();

private slots:
    void initServer();
    void acceptConnection();
    void recvMessage();
    void sendMessage(int state, Step step);
    void connectHost(QString ip);
    void getIP();
    void broadcast();
    void listen();
    void closeListen();
    void closeWrite();
    void processPendingDatagrams();

private:
    QTcpServer  *listenSocket;
    QTcpSocket  *readWriteSocket;
    QUdpSocket *SudpSocket, *LudpSocket;
    QString Sadress, Badress;
    QTimer m_timer;
    QString m_name;

    friend class MainWindow;
};

#endif // NETWORKSERVER_H
