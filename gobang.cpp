#include "gobang.h"
#include "ui_gobang.h"
#include "winwidget.h"
#include "ui_winwidget.h"
#include "enterdialog.h"
#include "ui_enterdialog.h"

#include <QPainter>
#include <QFileDialog>
#include <QPixmap>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QTextStream>

#include <algorithm>

Gobang::Gobang(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Gobang),
    m_plat(this),
    m_setPiece("./Music/chess.wav", this),
    m_win("./Music/win.wav", this),
    m_lose("./Music/lose.wav", this)
{
    ui->setupUi(this);
    m_plat.setGeometry(QRect(20, 20, 460, 462));
    m_plat.raise();
    m_plat.show();
    m_timer.setInterval(250);
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(timeOut()));
    connect(ui->back, SIGNAL(clicked(bool)), this, SLOT(on_back_click()));
    timeCount = -1;
    m_host = false;
    turn = 1;
    currentCamp = black;
    if(currentCamp == m_camp)
        ui->currentCamp->setText("请您落子");
    else
        ui->currentCamp->setText("轮到对方落子");
    wbStep[0] = 0;
    wbStep[1] = 0;
    wbTime[0] = 0;
    wbTime[1] = 0;
    m_noPress = false;
    m_size = 15;
    m_camp = white;
    connect(this, SIGNAL(win(int)), this, SLOT(setWin(int)));
}

Gobang::~Gobang()
{
    delete ui;
}

void Gobang::mousePressEvent(QMouseEvent *event)
{
    int x, y;
    if(event->x() >= 20 && event->x() < 20 + m_size*30 && event->y() >= 20 && event->y() < 40 + m_size*30 &&
            !m_noPress)
    {
        if(currentCamp != m_camp)
            return;
        x = (event->x() - 25) / 30;
        y = (event->y() - 25) / 30;
        if(!setPieces(Step(x, y, m_camp)))
            return;
        turn++;
        emit sendSignal(1, Step(x, y, m_camp));
    }
}

int Gobang::checkWin()
{
    for(int i = 0; i < m_size; ++i)
        for(int j = 0; j < m_size; ++j)
            if (board_[i][j].maxLian >= 5)
            {
                m_timer.stop();
                emit win(board_[i][j].camp);
                return board_[i][j].camp;
            }
    return 0;
}

bool Gobang::setPieces(const Step &step)
{
    if(board_[step.x][step.y].camp > 0)
        return false;
    else
    {
        m_plat.setPieces(step);
        m_plat.update();
        m_setPiece.play();
        board_[step.x][step.y].camp = step.camp;
        for(int xi = 0; xi < m_size; xi++)
            for(int yi = 0; yi < m_size; yi++)
                for(int i = 0; i < 4; i++)
                    board_[xi][yi].lian[i] = 0;
        for(int xi = 0; xi < m_size; xi++)
            for(int yi = 0; yi < m_size; yi++)
            {
                Pieces& tmp = board_[xi][yi];
                for(int i = 0; i < 4; i++)
                    tmp.lian[i] = (tmp.camp > 0);
                if(xi - 1 >= 0 && yi - 1 >= 0 && tmp.camp > 0)
                    tmp.lian[1] += (board_[xi-1][yi-1].camp == tmp.camp?board_[xi-1][yi-1].lian[1]:0);
                if(yi - 1 >= 0 && tmp.camp > 0)
                    tmp.lian[2] += (board_[xi][yi-1].camp == tmp.camp?board_[xi][yi-1].lian[2]:0);
                if(xi - 1 >= 0 && yi + 1 < m_size && tmp.camp > 0)
                    tmp.lian[0] += (board_[xi-1][yi+1].camp == tmp.camp?board_[xi-1][yi+1].lian[0]:0);
                if(xi - 1 >= 0 && tmp.camp > 0)
                    tmp.lian[3] += (board_[xi-1][yi].camp == tmp.camp?board_[xi-1][yi].lian[3]:0);
                for(int i = 0; i < 4; i++)
                    tmp.maxLian = tmp.maxLian>tmp.lian[i]?tmp.maxLian:tmp.lian[i];
            }
        qDebug() << board_[5][5].camp << board_[5][5].lian[0] <<  board_[5][5].lian[1] <<board_[5][5].lian[2] <<board_[5][5].lian[3];
        currentCamp = 3 - step.camp;
        if(currentCamp == m_camp)
            ui->currentCamp->setText("请您落子");
        else
            ui->currentCamp->setText("轮到对方落子");
        timeCount = -1;
        wbStep[step.camp-1]++;
        m_step.push_back(step);
        checkWin();
        return true;
    }
}

void Gobang::setHost()
{
    m_host = true;
    m_camp = black;
}

void Gobang::reStart()
{
    timeCount = -1;
    currentCamp = 2;
    if(!m_timer.isActive())
        m_timer.start();
    if(currentCamp == m_camp)
        ui->currentCamp->setText("请您落子");
    else
        ui->currentCamp->setText("轮到对方落子");
    wbStep[0] = 0;
    wbStep[1] = 0;
    wbTime[0] = 0;
    wbTime[1] = 0;
    m_plat.reStart();
    for(int i = 0; i < m_size; ++i)
        for(int j = 0; j < m_size; ++j)
            board_[i][j].clear();
    m_step.clear();
}

void Gobang::setWin(int camp)
{
    WinWidget *win = new WinWidget(WinWidget::win, this);
    m_noPress = true;
    connect(win, SIGNAL(isClosed(bool)), this, SLOT(changePress()));
    win->ui->bTime->setText(QString::number(wbTime[1]));
    win->ui->wTime->setText(QString::number(wbTime[0]));
    if(camp == m_camp)
    {
        QPixmap temp(":/Pic/GobangUi/win.png");
        win->ui->label->setGeometry(QRect(60, 40, temp.width()*0.4, temp.height()*0.4));
        win->ui->label->setPixmap(temp);
        win->show();
        m_win.play();
    }
    else
    {
        QPixmap temp(":/Pic/GobangUi/lose.png");
        win->ui->label->setGeometry(QRect(60, 40, temp.width()*0.4, temp.height()*0.4));
        win->ui->label->setPixmap(temp);
        win->show();
        m_lose.play();
    }
    connect(win, SIGNAL(isClosed(bool)), this, SLOT(reStart()));
}

void Gobang::timeOut()
{
    timeCount++;
    wbTime[currentCamp-1]++;
    ui->time->setText(QString::number(timeCount/4)+"s");
    if(timeCount == 80 && m_host == true)
    {
        currentCamp = 3 - currentCamp;
        emit sendSignal(currentCamp+1, Step(0, 0, 0));
        if(currentCamp == m_camp)
            ui->currentCamp->setText("请您落子");
        else
            ui->currentCamp->setText("轮到对方落子");
        timeCount = -1;
    }
}

void Gobang::changeCamp(int camp)
{
     currentCamp = camp;
     if(currentCamp == m_camp)
         ui->currentCamp->setText("请您落子");
     else
         ui->currentCamp->setText("轮到对方落子");
     timeCount = -1;
}

void Gobang::on_recall_clicked()
{
    if(m_step.size() > 0 && wbStep[m_camp-1] > 0)
    {
        emit sendSignal(7, Step(0,0,0));
        WinWidget *win = new WinWidget(WinWidget::waitForRecall, this);
        connect(this, SIGNAL(haveAgreed()), &win->time, SLOT(start()));
        connect(this, SIGNAL(haveAgreed()), win, SLOT(deleteLater()));
        connect(win, SIGNAL(isClosed(bool)), &m_timer, SLOT(start()));
        connect(this, SIGNAL(isRefused()), win, SLOT(deleteLater()));
        win->show();
        m_timer.stop();
    }
}

void Gobang::on_recall()
{
    currentCamp = m_camp;
    emit haveAgreed();
    if(currentCamp == m_camp)
        ui->currentCamp->setText("请您落子");
    else
        ui->currentCamp->setText("轮到对方落子");
    bool flag = true;
    timeCount = -1;
    wbStep[m_camp-1]--;
    while(flag && wbStep[m_camp-1] >= 0)
    {
        if(m_step.back().camp == m_camp)
            flag = false;
        board_[m_step.back().x][m_step.back().y].clear();
        m_plat.board_[m_step.back().x][m_step.back().y] = 0;
        m_step.pop_back();
    }
    m_plat.update();
    emit sendSignal(4, Step(0, 0, m_camp));
}

void Gobang::forRecall()
{
    WinWidget *win = new WinWidget(WinWidget::recall, this);
    m_timer.stop();
    m_noPress = true;
    connect(win, SIGNAL(isClosed(bool)), this, SLOT(changePress()));
    connect(win, SIGNAL(isClosed(bool)), &m_timer, SLOT(start()));
    win->show();
    connect(win, SIGNAL(agreeRecall()), this, SLOT(agreeRecall()));
    connect(win, SIGNAL(haveRefused()), this, SLOT(refuse()));
}

void Gobang::forExit()
{
    WinWidget *win = new WinWidget(WinWidget::exit, this);
    m_timer.stop();
    m_noPress = true;
    connect(win, SIGNAL(isClosed(bool)), this, SLOT(changePress()));
    connect(win, SIGNAL(isClosed(bool)), &m_timer, SLOT(start()));
    win->show();
    connect(win, SIGNAL(agreeExit()), this, SLOT(agreeExit()));
    connect(win, SIGNAL(haveRefused()), this, SLOT(refuse()));
}

void Gobang::agreeRecall()
{
    emit sendSignal(8, Step(0,0,0));
}

void Gobang::agreeExit()
{
    emit sendSignal(10, Step(0, 0, 0));
}

void Gobang::changeCurrentState(int camp, int wbNumw, int wbNumb)
{
    currentCamp = camp;
    wbStep[0] = wbNumw;
    wbStep[1] = wbNumb;
}

void Gobang::recallDone(int camp)
{
    currentCamp = camp;
    if(currentCamp == m_camp)
        ui->currentCamp->setText("请您落子");
    else
        ui->currentCamp->setText("轮到对方落子");
    bool flag = true;
    timeCount = -1;
    wbStep[camp-1]--;
    while(flag && wbStep[camp-1] >= 0)
    {
        if(m_step.back().camp == camp)
            flag = false;
        board_[m_step.back().x][m_step.back().y].clear();
        m_plat.board_[m_step.back().x][m_step.back().y] = 0;
        m_step.pop_back();
    }
    m_plat.update();
}

void Gobang::saveGame(QString fileName)
{
    QDir dir;
    dir.mkpath("./Save/");
    QFile file("./Save/" + fileName + ".sav");
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    out << currentCamp << " " << wbStep[0] << " " << wbStep[1] << " " << m_step.size() << endl;
    for(int i = 0; i < (int)m_step.size(); ++i)
        out << m_step[i].x << " " << m_step[i].y << " " << m_step[i].camp << endl;
    file.close();
}

void Gobang::on_save_clicked()
{
    if(m_host)
    {
        WinWidget *win = new WinWidget(WinWidget::save, this);
        m_noPress = true;
        connect(win, SIGNAL(isClosed(bool)), this, SLOT(changePress()));
        connect(win, SIGNAL(saveFile(QString)), this, SLOT(saveGame(QString)));
        win->show();
    }
}

void Gobang::on_load_clicked()
{
    if(m_host)
    {
        WinWidget *win = new WinWidget(WinWidget::load, this);
        m_noPress = true;
        connect(win, SIGNAL(isClosed(bool)), this, SLOT(changePress()));
        win->show();
        QFileDialog temp(this);
        QString path;
        temp.setWindowTitle(tr("请打开存档文件"));
        temp.setDirectory("./Save");
        temp.setNameFilter(tr("存档文件(*.sav)"));
        if(temp.exec() == QDialog::Accepted)
            path = temp.selectedFiles()[0];
        QFile file(path);
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        QTextStream in(&file);
        int size;
        int temp1, temp2, temp3;
        in >> temp1 >> temp2 >> temp3 >> size;
        reStart();
        for(int i = 0; i < size; ++i)
        {
            int x, y, camp;
            in >> x >> y >> camp;
            m_step.push_back(Step(x, y, camp));
            setPieces(m_step.back());
        }
        emit sendSignal(5, Step(0, 0, 0));
        m_plat.update();
        for(int i = 0; i < (int)m_step.size(); ++i)
            emit sendSignal(1, m_step[i]);
        emit sendSignal(6, Step(temp1, temp2, temp3));
        currentCamp = temp1;
        wbStep[0] = temp2;
        wbStep[1] = temp3;
    }
}

void Gobang::on_back_click()
{
    emit sendSignal(9, Step(0,0,0));
    WinWidget *win = new WinWidget(WinWidget::waitForExit, this);
    connect(win, SIGNAL(isClosed(bool)), &m_timer, SLOT(start()));
    connect(this, SIGNAL(haveExited()), win, SLOT(deleteLater()));
    connect(this, SIGNAL(isRefused()), win, SLOT(deleteLater()));
    win->show();
    m_timer.stop();
}
