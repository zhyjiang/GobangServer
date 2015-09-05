#include "winwidget.h"
#include "ui_winwidget.h"
#include <QBrush>
#include <QPainter>
#include <QPixmap>
#include <QColor>
#include <QDebug>
#include <QPen>

WinWidget::WinWidget(int _type, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WinWidget),
    m_effect(this)
{
    ui->setupUi(this);
    m_opacity = 0;
    m_effect.setOpacity(m_opacity);
    setGraphicsEffect(&m_effect);
    time.setInterval(20);
    time.start();
    connect(&time, SIGNAL(timeout()), this, SLOT(thicker()));
    ui->bTime->hide();
    ui->wTime->hide();
    ui->bTimeLabel->hide();
    ui->wTimeLabel->hide();
    ui->bTimeLabel->setScaledContents(true);
    ui->wTimeLabel->setScaledContents(true);
    ui->label->setScaledContents(true);
    ui->iconLabel->setScaledContents(true);
    ui->saveLabel->setScaledContents(true);
    switch (_type)
    {
        case load:
        {
            ui->saveLabel->hide();
            ui->saveEdit->hide();
            ui->refuse->hide();
            QPixmap temp(":/Pic/GobangUi/load.png");
            ui->label->setGeometry(QRect(100, 70, temp.width()*0.6, temp.height()*0.6));
            ui->label->setPixmap(temp);
            temp.load(":/Pic/GobangUi/loadIcon.png");
            ui->iconLabel->setGeometry(QRect(80, 80, temp.width(), temp.height()));
            ui->iconLabel->setPixmap(temp);
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
            QPixmap temp(":/Pic/GobangUi/save.png");
            ui->label->setGeometry(QRect(100, 90, temp.width()*0.6, temp.height()*0.6));
            ui->label->setPixmap(temp);
            temp.load(":/Pic/GobangUi/saveIcon.png");
            ui->iconLabel->setGeometry(QRect(80, 70, temp.width()*0.8, temp.height()*0.8));
            ui->iconLabel->setPixmap(temp);
            temp.load(":/Pic/GobangUi/saveLabel.png");
            ui->saveLabel->setGeometry(QRect(40, 220, temp.width()*0.8, temp.height()*0.8));
            ui->saveLabel->setPixmap(temp);
            break;
        }
        case recall:
        {
            ui->saveLabel->hide();
            ui->saveEdit->hide();
            ui->refuse->show();
            QPixmap temp(":/Pic/GobangUi/recall.png");
            ui->label->setGeometry(QRect(100, 90, temp.width()*0.6, temp.height()*0.6));
            ui->label->setPixmap(temp);
            temp.load(":/Pic/GobangUi/chinaz258.png");
            ui->iconLabel->setGeometry(QRect(80, 50, temp.width()*0.8, temp.height()*0.8));
            ui->iconLabel->setPixmap(temp);
            connect(ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(agree()));
            break;
        }
        case disconnect:
        {
            ui->saveLabel->hide();
            ui->saveEdit->hide();
            ui->refuse->hide();
            QPixmap temp(":/Pic/GobangUi/disconnect.png");
            ui->label->setGeometry(QRect(100, 100, temp.width()*0.6, temp.height()*0.6));
            ui->label->setPixmap(temp);
            temp.load(":/Pic/GobangUi/chinaz259.png");
            ui->iconLabel->setGeometry(QRect(80, 20, temp.width()*0.8, temp.height()*0.8));
            ui->iconLabel->setPixmap(temp);
            break;
        }
        case waitForRecall:
        {
            ui->saveLabel->hide();
            ui->saveEdit->hide();
            ui->refuse->hide();
            ui->pushButton->hide();
            QPixmap temp(":/Pic/GobangUi/waitRespond.png");
            ui->label->setGeometry(QRect(100, 100, temp.width()*0.6, temp.height()*0.6));
            ui->label->setPixmap(temp);
            temp.load(":/Pic/GobangUi/EARST_20070731B_PNG03.png");
            ui->iconLabel->setGeometry(QRect(40, 90, temp.width()*0.8, temp.height()*0.8));
            ui->iconLabel->setPixmap(temp);
            break;
        }
        case exit:
        {
            ui->saveLabel->hide();
            ui->saveEdit->hide();
            ui->refuse->show();
            connect(ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(isAgreeExit()));
            QPixmap temp(":/Pic/GobangUi/exit.png");
            ui->label->setGeometry(QRect(100, 100, temp.width()*0.6, temp.height()*0.6));
            ui->label->setPixmap(temp);
            temp.load(":/Pic/GobangUi/EARST_20070731B_PNG33.png");
            ui->iconLabel->setGeometry(QRect(40, 90, temp.width()*0.8, temp.height()*0.8));
            ui->iconLabel->setPixmap(temp);
            break;
        }
        case waitForExit:
        {
            ui->saveLabel->hide();
            ui->saveEdit->hide();
            ui->refuse->hide();
            ui->pushButton->hide();
            QPixmap temp(":/Pic/GobangUi/waitRespond.png");
            ui->label->setGeometry(QRect(100, 100, temp.width()*0.6, temp.height()*0.6));
            ui->label->setPixmap(temp);
            temp.load(":/Pic/GobangUi/EARST_20070731B_PNG03.png");
            ui->iconLabel->setGeometry(QRect(40, 90, temp.width()*0.8, temp.height()*0.8));
            ui->iconLabel->setPixmap(temp);
        }
    }
    connect(ui->refuse, SIGNAL(clicked()), this, SLOT(refuse()));
    connect(ui->refuse, SIGNAL(clicked(bool)), &time, SLOT(start()));
    connect(ui->pushButton, SIGNAL(clicked(bool)), &time, SLOT(start()));
}

WinWidget::~WinWidget()
{
    emit isClosed(true);
    delete ui;
}

void WinWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QBrush brush(QColor(255, 255, 255, 200), Qt::SolidPattern);
    painter.setPen(Qt::NoPen);
    painter.setBrush(brush);
    painter.drawRect(0, 0, 680, 515);
}

void WinWidget::saveGame()
{
    emit saveFile(ui->saveEdit->text()==""?"Untitled":ui->saveEdit->text());
}

void WinWidget::agree()
{
    emit agreeRecall();
}

void WinWidget::isAgreeExit()
{
    emit agreeExit();
}

void WinWidget::refuse()
{
    emit haveRefused();
}

void WinWidget::thicker()
{
    m_opacity += 0.06;
    m_effect.setOpacity(m_opacity);
    if(m_opacity > 1)
    {
        time.stop();
        time.disconnect();
        connect(&time, SIGNAL(timeout()), this, SLOT(thinner()));
    }
}

void WinWidget::thinner()
{
    m_opacity -= 0.06;
    m_effect.setOpacity(m_opacity);
    if(m_opacity < 0)
    {
        time.stop();
        close();
        deleteLater();
    }
}
