#include "gobang.h"
#include "ui_gobang.h"
#include "winwidget.h"
#include "ui_winwidget.h"
#include "enterdialog.h"
#include "ui_enterdialog.h"

#include <QPainter>
#include <QFileDialog>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QTextStream>

#include <algorithm>

Gobang::Gobang(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Gobang)
{
    ui->setupUi(this);

    m_timer.setInterval(1000);
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(timeOut()));
    timeCount = -1;
    m_host = false;
    turn = 1;
    currentCamp = black;
    ui->currentCamp->setText(QString::number(black));
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

void Gobang::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);
    for (int i = 0; i < m_size; i++)
    {
        p.drawLine(40, 40 + i * 30, 40 + (m_size-1)*30, 40 + i * 30);
        p.drawLine(40 + i * 30, 40, 40 + i * 30, 40 + (m_size-1)*30);
    }

    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    for (int i = 0; i < m_size; i++)
        for (int j = 0; j < m_size; j++)
        {
            if (board_[i][j].camp == 2)
            {
                brush.setColor(Qt::black);
                p.setBrush(brush);
                p.setPen(QPen(Qt::black));
                p.drawEllipse(QPoint(40 + i * 30, 40 + j * 30), 12, 12);
            }
            else if (board_[i][j].camp == 1)
            {
                brush.setColor(Qt::white);
                p.setBrush(brush);
                p.setPen(QPen(Qt::white));
                p.drawEllipse(QPoint(40 + i * 30, 40 + j * 30), 12, 12);
            }
        }
}

void Gobang::mousePressEvent(QMouseEvent *event)
{
    int x, y;
    if(event->x() >= 20 && event->x() < 20 + m_size*30 && event->y() >= 20 && event->y() < 40 + m_size*30 &&
            !m_noPress)
    {
        if(m_step.size() == 0 && m_host == false)
            return;
        if(m_step.size() > 0)
            if(currentCamp != m_camp)
                return;
        x = (event->x() - 25) / 30;
        y = (event->y() - 25) / 30;
        if(!setPieces(Step(x, y, m_camp)))
            return;
        turn++;
        emit setPiece(1, Step(x, y, m_camp));
        update();
    }
}

int Gobang::checkWin()
{
    for(int i = 0; i < m_size; ++i)
        for(int j = 0; j < m_size; ++j)
            if (board_[i][j].maxLian >= 5)
            {
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
        board_[step.x][step.y].camp = step.camp;
        for(int xi = 0; xi < m_size; xi++)
            for(int yi = 0; yi < m_size; yi++)
            {
                Pieces& tmp = board_[xi][yi];
                for(int i = 0; i < 4; i++)
                    tmp.lian[i] = 0;
                if(xi - 1 >= 0 && yi - 1 >= 0 && tmp.camp > 0)
                    tmp.lian[1] += (board_[xi-1][yi-1].camp == tmp.camp?board_[xi-1][yi-1].lian[1]+1:1);
                else tmp.lian[1] = (tmp.camp > 0);
                if(yi - 1 >= 0 && tmp.camp > 0)
                    tmp.lian[2] += (board_[xi][yi-1].camp == tmp.camp?board_[xi][yi-1].lian[2]+1:1);
                else tmp.lian[2] = (tmp.camp > 0);
                if(xi + 1 < m_size && yi - 1 >= 0 && tmp.camp > 0)
                    tmp.lian[0] += (board_[xi+1][yi-1].camp == tmp.camp?board_[xi+1][yi-1].lian[0]+1:1);
                else tmp.lian[0] = (tmp.camp > 0);
                if(xi - 1 >= 0 && tmp.camp > 0)
                    tmp.lian[3] += (board_[xi-1][yi].camp == tmp.camp?board_[xi-1][yi].lian[3]+1:1);
                else tmp.lian[3] = (tmp.camp > 0);
                for(int i = 0; i < 4; i++)
                    tmp.maxLian = tmp.maxLian>tmp.lian[i]?tmp.maxLian:tmp.lian[i];
            }
        currentCamp = 3 - step.camp;
        ui->currentCamp->setText(QString::number(currentCamp));
        timeCount = -1;
        wbStep[step.camp-1]++;
        qDebug() << "white" << wbStep[0];
        qDebug() << "black" << wbStep[1];
        update();
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
    wbStep[0] = 0;
    wbStep[1] = 0;
    wbTime[0] = 0;
    wbTime[1] = 0;
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
        win->ui->label->setText("你赢啦！！");
        win->show();
    }
    else
    {
        win->ui->label->setText("你输了。。。");
        win->show();
    }
    connect(win, SIGNAL(isClosed(bool)), this, SLOT(reStart()));
}

void Gobang::timeOut()
{
    timeCount++;
    wbTime[currentCamp-1]++;
    ui->time->setText(QString::number(timeCount));
    if(timeCount == 20 && m_host == true)
    {
        currentCamp = 3 - currentCamp;
        emit timeOut(currentCamp+1, Step(0, 0, 0));
        ui->currentCamp->setText(QString::number(currentCamp));
        timeCount = -1;
    }
}

void Gobang::changeCamp(int camp)
{
     currentCamp = camp;
     ui->currentCamp->setText(QString::number(currentCamp));
     timeCount = -1;
}

void Gobang::on_recall_clicked()
{
    if(m_step.size() > 0 && wbStep[m_camp-1] > 0)
    {
        qDebug() << m_camp;
        emit askForRecall(7, Step(0,0,0));
    }
}

void Gobang::on_recall()
{
    currentCamp = m_camp;
    ui->currentCamp->setText(QString::number(currentCamp));
    bool flag = true;
    timeCount = -1;
    wbStep[m_camp-1]--;
    while(flag && wbStep[m_camp-1] >= 0)
    {
        if(m_step.back().camp == m_camp)
            flag = false;
        qDebug() << m_camp << flag;
        board_[m_step.back().x][m_step.back().y].clear();
        m_step.pop_back();
    }
    update();
    emit recall(4, Step(0, 0, m_camp));
}

void Gobang::forRecall()
{
    WinWidget *win = new WinWidget(WinWidget::recall, this);
    m_noPress = true;
    connect(win, SIGNAL(isClosed(bool)), this, SLOT(changePress()));
    win->show();
    connect(win, SIGNAL(agreeRecall()), this, SLOT(agreeRecall()));
}

void Gobang::agreeRecall()
{
    emit isAgreeRecall(8, Step(0,0,0));
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
    ui->currentCamp->setText(QString::number(currentCamp));
    bool flag = true;
    timeCount = -1;
    wbStep[camp-1]--;
    while(flag && wbStep[camp-1] >= 0)
    {
        if(m_step.back().camp == camp)
            flag = false;
        board_[m_step.back().x][m_step.back().y].clear();
        m_step.pop_back();
    }
    update();
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
        emit reStartGame(5, Step(0, 0, 0));
        update();
        for(int i = 0; i < (int)m_step.size(); ++i)
            emit setPiece(1, m_step[i]);
        emit changeState(6, Step(temp1, temp2, temp3));
        currentCamp = temp1;
        wbStep[0] = temp2;
        wbStep[1] = temp3;
    }
}
