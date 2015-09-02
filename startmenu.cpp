#include "startmenu.h"
#include "ui_startmenu.h"
#include <QTextStream>

StartMenu::StartMenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StartMenu)
{
    ui->setupUi(this);
    connect(ui->playerList, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(activeItem(QListWidgetItem*)));
}

StartMenu::~StartMenu()
{
    delete ui;
}

void StartMenu::activeItem(QListWidgetItem* temp)
{
    QString str = temp->text();
    QTextStream in(&str);
    in >> currentIp;
    in >> currentIp;
}
