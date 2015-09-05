#include "waitwidget.h"
#include "ui_waitwidget.h"

waitWidget::waitWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::waitWidget)
{
    ui->setupUi(this);
    ui->waitLabel->hide();
    ui->waitLabel->setScaledContents(true);
    QPixmap temp(":/Pic/GobangUi/wait.png");
    ui->waitLabel->setGeometry(QRect(100, 50, temp.width()*0.6, temp.height()*0.6));
    ui->waitLabel->setPixmap(temp);
}

waitWidget::~waitWidget()
{
    delete ui;
}
