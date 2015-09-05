#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_startmenu.h"
#include "ui_waitWidget.h"
#include "ui_gobang.h"
#include "enterdialog.h"
#include "winwidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_server(new NetworkServer(this)),
    m_ipList(),
    m_bgm(this)
{
    ui->setupUi(this);
    m_bgm.setMedia(QUrl::fromLocalFile("./Music/thestonemasons.wav"));
    m_bgm.setVolume(50);
    m_bgm.play();
    connect(&m_bgm, SIGNAL(stateChanged(QMediaPlayer::State)), &m_bgm, SLOT(play()));
    setFixedSize(width(), height());
    m_gobang = new Gobang(this);
    m_startMenu = new StartMenu(this);
    m_waitWidget = new waitWidget(this);
    setCursor(QCursor(QPixmap(":/Pic/arrows.png"), 1, 1));
    m_gobang->hide();
    m_waitWidget->hide();
    m_startMenu->show();
    currentWidget = m_startMenu;
    m_server->listen();
    m_server->getIP();
    m_waitWidget->ui->ipLabel->setText("您的IP是:" + m_server->Sadress);
    m_timer.setInterval(1000);
    currentState = m_server->readWriteSocket->state();
    m_time1.setInterval(1000); m_time1.start();
    connect(&m_time1, SIGNAL(timeout()), this, SLOT(checkState()));
    m_timer.start();
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(checkUnactive()));

    connect(m_startMenu->ui->startServer, SIGNAL(clicked()), this, SLOT(setWaiting()));
    connect(m_startMenu->ui->startServer, SIGNAL(clicked()), m_gobang, SLOT(setHost()));
    connect(m_startMenu->ui->startServer, SIGNAL(clicked()), m_server, SLOT(closeListen()));
    connect(m_startMenu->ui->startServer, SIGNAL(clicked()), m_server, SLOT(initServer()));

    connect(m_startMenu->ui->startClient, SIGNAL(clicked()), this, SLOT(connectHost()));
    connect(m_startMenu->ui->startClient, SIGNAL(clicked()), m_server, SLOT(closeListen()));

    connect(m_waitWidget->ui->ok, SIGNAL(clicked()), this, SLOT(broadcast()));
    connect(m_waitWidget->ui->ok, SIGNAL(clicked()), m_waitWidget->ui->setBox, SLOT(hide()));
    connect(m_waitWidget->ui->ok, SIGNAL(clicked()), m_waitWidget->ui->waitLabel, SLOT(show()));
    connect(m_waitWidget->ui->back, SIGNAL(clicked()), m_server, SLOT(closeWrite()));
    connect(m_waitWidget->ui->back, SIGNAL(clicked()), m_server, SLOT(listen()));
    connect(m_waitWidget->ui->back, SIGNAL(clicked()), this, SLOT(setStart()));

    connect(m_gobang, SIGNAL(setPiece(int, Step)), m_server, SLOT(sendMessage(int, Step)));
    connect(m_gobang, SIGNAL(timeOut(int, Step)), m_server, SLOT(sendMessage(int,Step)));
    connect(m_gobang, SIGNAL(recall(int, Step)), m_server, SLOT(sendMessage(int,Step)));
    connect(m_gobang, SIGNAL(reStartGame(int, Step)), m_server, SLOT(sendMessage(int,Step)));
    connect(m_gobang, SIGNAL(changeState(int,Step)), m_server, SLOT(sendMessage(int,Step)));
    connect(m_gobang, SIGNAL(askForRecall(int, Step)), m_server, SLOT(sendMessage(int,Step)));
    connect(m_gobang, SIGNAL(askForExit(int, Step)), m_server, SLOT(sendMessage(int,Step)));
    connect(m_gobang, SIGNAL(isAgreeRecall(int, Step)), m_server, SLOT(sendMessage(int,Step)));
    connect(m_gobang, SIGNAL(isAgreeExit(int, Step)), m_server, SLOT(sendMessage(int,Step)));
    connect(m_gobang, SIGNAL(haveRefused(int, Step)), m_server, SLOT(sendMessage(int,Step)));

    connect(m_server, SIGNAL(setPieces(Step)), m_gobang, SLOT(setPieces(const Step&)));
    connect(m_server, SIGNAL(findPlayer(QString)), this, SLOT(findPlayer(QString)));
    connect(m_server, SIGNAL(changeCamp(int)), m_gobang, SLOT(changeCamp(int)));
    connect(m_server, SIGNAL(recall(int)), m_gobang, SLOT(recallDone(int)));
    connect(m_server, SIGNAL(reStart()), m_gobang, SLOT(reStart()));
    connect(m_server, SIGNAL(agreeRecall()), m_gobang, SLOT(on_recall()));
    connect(m_server, SIGNAL(askForRecall()), m_gobang, SLOT(forRecall()));
    connect(m_server, SIGNAL(agreeExit()), this, SLOT(setStart()));
    connect(m_server, SIGNAL(agreeExit()), m_gobang, SLOT(on_exit()));
    connect(m_server, SIGNAL(refuse()), m_gobang, SLOT(on_refuse()));
    connect(m_server, SIGNAL(agreeExit()), m_server, SLOT(listen()));
    connect(m_server, SIGNAL(askForExit()), m_gobang, SLOT(forExit()));
    connect(m_server, SIGNAL(changeState(int, int, int)), m_gobang, SLOT(changeCurrentState(int, int, int)));
    connect(m_server->listenSocket, SIGNAL(newConnection()), this, SLOT(setGobang()));
    connect(m_server->listenSocket, SIGNAL(newConnection()), m_server, SLOT(closeWrite()));
    connect(m_server->listenSocket, SIGNAL(newConnection()), m_server, SLOT(closeListen()));
//    connect(m_server->readWriteSocket, SIGNAL(connected()), this, SLOT(setGobang()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setWaiting()
{
    currentWidget->hide();
    m_waitWidget->show();
    currentWidget = m_waitWidget;
    if(m_name.size() > 0)
        broadcast();
}

void MainWindow::setGobang()
{
    currentWidget->hide();
    m_gobang->reStart();
    m_gobang->show();
    m_gobang->m_timer.start();
    currentWidget = m_gobang;
    m_timer.stop();
}

void MainWindow::setStart()
{
    currentWidget->hide();
    m_startMenu->show();
    m_server->listen();
    m_server->readWriteSocket->abort();
    connect(m_server->readWriteSocket, SIGNAL(connected()), this, SLOT(setGobang()));
    m_gobang->m_host = false;
    currentWidget = m_startMenu;
    m_gobang->reStart();
    m_timer.start();
}

void MainWindow::checkState()
{
    qDebug() << currentState;
    if(currentState == QAbstractSocket::ConnectedState &&
            m_server->readWriteSocket->state() == QAbstractSocket::UnconnectedState)
    {
        WinWidget *win = new WinWidget(WinWidget::disconnect, this);
        win->show();
        setStart();
    }
    else if(currentState == QAbstractSocket::UnconnectedState &&
            m_server->readWriteSocket->state() == QAbstractSocket::ConnectedState)
    {
        setGobang();
        m_server->closeListen();
        m_server->closeWrite();
    }
    currentState = m_server->readWriteSocket->state();
}

void MainWindow::findPlayer(QString IP)
{
    bool flag = false;
    QTextStream in(&IP);
    QString name, ip;
    in >> name;
    in >> ip;
    int count = 0;
    foreach (QString temp, m_ipList)
    {
        if(ip == temp)
        {
            m_waitList[count] = 3;
            flag = true;
            break;
        }
        count++;
    }
    if(!flag && ip != m_server->Sadress)
    {
        m_nameList.push_back(name);
        m_ipList.push_back(ip);
        m_waitList.push_back(3);
        refresh();
    }
}

void MainWindow::refresh()
{
    m_startMenu->ui->playerList->clear();
    for (int i = 0; i < m_ipList.size(); i++)
        m_startMenu->ui->playerList->addItem(m_nameList[i] + " " + m_ipList[i]);
}

void MainWindow::broadcast()
{
    if(m_name.size() == 0)
    {
        if(m_waitWidget->ui->nameEdit->text() != "")
            m_name = m_waitWidget->ui->nameEdit->text();
        else
            m_name = "Unamed";
    }
    m_server->m_timer.start();
    m_server->m_name = m_name;
    m_server->broadcast();
}

void MainWindow::checkUnactive()
{
    for(int i = 0; i < m_waitList.size(); ++i)
    {
        m_waitList[i]--;
        if(m_waitList[i] == -1)
        {
            m_waitList.removeAt(i);
            m_ipList.removeAt(i);
            m_nameList.removeAt(i);
            refresh();
        }
    }
}

void MainWindow::connectHost()
{
    m_gobang->m_camp = Gobang::white;
    if(m_startMenu->currentIp.size() > 0)
    {
        m_server->connectHost(m_startMenu->currentIp);
    }
    else
    {
        enterDialog tempDialog(this);
        connect(&tempDialog, SIGNAL(setIP(QString)), m_server, SLOT(connectHost(QString)));
        tempDialog.exec();
    }
}
