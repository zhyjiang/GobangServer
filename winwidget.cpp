#include "winwidget.h"
#include "ui_winwidget.h"
#include <QBrush>
#include <QPainter>
#include <QColor>
#include <QDebug>
#include <QPen>

WinWidget::WinWidget(int _type, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WinWidget),
    m_effect(this)
{
    ui->setupUi(this);
    ui->bTime->hide();
    ui->wTime->hide();
    ui->bTimeLabel->hide();
    ui->wTimeLabel->hide();
    switch (_type)
    {
        case load:
        {
            ui->saveLabel->hide();
            ui->saveEdit->hide();
            ui->refuse->hide();
            break;
        }
        case win:
        {
            ui->saveLabel->hide();
            ui->saveEdit->hide();
            ui->refuse->hide();
            ui->bTime->show();
            ui->wTime->show();
            ui->bTimeLabel->show();
            ui->wTimeLabel->show();
            break;
        }
        case save:
        {
            connect(ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(saveGame()));
            ui->refuse->hide();
            break;
        }
        case recall:
        {
            ui->saveLabel->hide();
            ui->saveEdit->hide();
            connect(ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(agree()));
        }
        case disconnect:
        {
            ui->saveLabel->hide();
            ui->saveEdit->hide();
            ui->refuse->hide();
        }
    }
    connect(ui->refuse, SIGNAL(clicked(bool)), this, SLOT(close()));
    connect(ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(close()));
    connect(ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(deleteLater()));
}

WinWidget::~WinWidget()
{
    emit isClosed(true);
    delete ui;
}

void WinWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QBrush brush(QColor(0, 0, 0, 100), Qt::SolidPattern);
    painter.setPen(Qt::NoPen);
    painter.setBrush(brush);
    painter.drawRect(0, 0, 680, 515);
}

void WinWidget::saveGame()
{
    emit saveFile(ui->saveEdit->text());
}

void WinWidget::agree()
{
    emit agreeRecall();
}
