#include "waitwidget.h"
#include "ui_waitwidget.h"

waitWidget::waitWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::waitWidget)
{
    ui->setupUi(this);
    ui->waitLabel->hide();
}

waitWidget::~waitWidget()
{
    delete ui;
}
