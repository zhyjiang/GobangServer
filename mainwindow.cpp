#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_startmenu.h"
#include "ui_waitWidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_server(this),
    m_ipList()
{
    ui->setupUi(this);
    m_gobang = new Gobang(this);
    m_startMenu = new StartMenu(this);
    m_stack = new QStackedWidget(this);
    m_waitWidget = new waitWidget;
    setCentralWidget(m_stack);
    m_stack->addWidget(m_waitWidget);
    m_stack->addWidget(m_gobang);
    m_stack->addWidget(m_startMenu);
    m_stack->setCurrentWidget(m_startMenu);
    m_server.listen();
    m_server.getIP();
    m_waitWidget->ui->ipLabel->setText("您的IP是:" + m_server.Sadress);
    m_timer.setInterval(1000);
    m_timer.start();
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(checkUnactive()));

    connect(m_startMenu->ui->startServer, SIGNAL(clicked()), this, SLOT(setWaiting()));
    connect(m_startMenu->ui->startServer, SIGNAL(clicked()), m_gobang, SLOT(setHost()));
    connect(m_startMenu->ui->startServer, SIGNAL(clicked()), &m_server, SLOT(closeListen()));
    connect(m_startMenu->ui->startServer, SIGNAL(clicked()), &m_server, SLOT(initServer()));

    connect(m_startMenu->ui->startClient, SIGNAL(clicked()), this, SLOT(setGobang()));
    connect(m_startMenu->ui->startClient, SIGNAL(clicked()), this, SLOT(connectHost()));
    connect(m_startMenu->ui->startClient, SIGNAL(clicked()), &m_server, SLOT(closeListen()));

    connect(m_startMenu->ui->refresh, SIGNAL(clicked()), this, SLOT(refresh()));

    connect(m_waitWidget->ui->ok, SIGNAL(clicked()), this, SLOT(broadcast()));
    connect(m_waitWidget->ui->ok, SIGNAL(clicked()), m_waitWidget->ui->setBox, SLOT(hide()));
    connect(m_waitWidget->ui->ok, SIGNAL(clicked()), m_waitWidget->ui->waitLabel, SLOT(show()));
    connect(m_waitWidget->ui->back, SIGNAL(clicked()), &m_server, SLOT(closeWrite()));
    connect(m_waitWidget->ui->back, SIGNAL(clicked()), &m_server, SLOT(listen()));
    connect(m_waitWidget->ui->back, SIGNAL(clicked()), this, SLOT(setStart()));

    connect(m_gobang, SIGNAL(setPiece(int, Step)), &m_server, SLOT(sendMessage(int, Step)));
    connect(&m_server, SIGNAL(setPieces(Step)), m_gobang, SLOT(setPieces(const Step&)));
    connect(&m_server, SIGNAL(findPlayer(QString)), this, SLOT(findPlayer(QString)));
    connect(m_server.listenSocket, SIGNAL(newConnection()), this, SLOT(setGobang()));
    connect(m_server.listenSocket, SIGNAL(newConnection()), &m_server, SLOT(closeWrite()));
    connect(m_server.listenSocket, SIGNAL(newConnection()), &m_server, SLOT(closeListen()));

    m_stack->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setWaiting()
{
    m_stack->setCurrentWidget(m_waitWidget);
    if(m_name.size() > 0)
        broadcast();
}

void MainWindow::setGobang()
{
    m_stack->setCurrentWidget(m_gobang);
    m_timer.stop();
}

void MainWindow::setStart()
{
    m_stack->setCurrentWidget(m_startMenu);
        qDebug() << 1;
    m_gobang->m_host = false;
    m_gobang->m_camp = Gobang::white;
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
    if(!flag && ip != m_server.Sadress)
    {
        m_nameList.push_back(name);
        m_ipList.push_back(ip);
        m_waitList.push_back(3);
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
    m_server.m_timer.start();
    m_server.m_name = m_name;
    m_server.broadcast();
}

void MainWindow::checkUnactive()
{
    for(int i = 0; i < m_waitList.size(); ++i)
    {
        m_waitList[i]--;
        if(m_waitList[i] == -1)
        {
            qDebug() << i;
            m_waitList.removeAt(i);
            m_ipList.removeAt(i);
            m_nameList.removeAt(i);
        }
    }
}

void MainWindow::connectHost()
{
    if(m_startMenu->currentIp.size() > 0)
        m_server.connectHost(m_startMenu->currentIp);
    else
    {

    }
}
