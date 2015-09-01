#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_startmenu.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_gobang = new Gobang(this);
    m_startMenu = new StartMenu(this);
    m_stack = new QStackedWidget(this);
    setCentralWidget(m_stack);
    m_stack->addWidget(m_gobang);
    m_stack->addWidget(m_startMenu);
    m_stack->setCurrentWidget(m_startMenu);
    connect(m_startMenu->ui->startServer, SIGNAL(clicked()), this, SLOT(setGobang()));
    connect(m_startMenu->ui->startServer, SIGNAL(clicked()), m_gobang, SLOT(setHost()));
    connect(m_startMenu->ui->startServer, SIGNAL(clicked()), m_gobang->getNetworkServer(), SLOT(initServer()));
    connect(m_startMenu->ui->startClient, SIGNAL(clicked()), this, SLOT(setGobang()));
    connect(m_startMenu->ui->startClient, SIGNAL(clicked()), m_gobang->getNetworkServer(), SLOT(connectHost()));
    m_stack->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setGobang()
{
    m_stack->setCurrentIndex(0);
}
