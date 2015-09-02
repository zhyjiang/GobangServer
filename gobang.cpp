#include "gobang.h"
#include "ui_gobang.h"

#include <QPainter>
#include <QDebug>

#include <algorithm>

Gobang::Gobang(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Gobang)
{
    ui->setupUi(this);

    m_host = false;
    turn = 1;
    m_size = 15;
    m_camp = white;
    resize(40 + m_size*30, 40 + m_size*30);


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
    if(event->x() >= 20 && event->x() < 40 + m_size*30 && event->y() >= 20 && event->y() < 40 + m_size*30)
    {
        if(m_step.size() > 0)
            if(m_step.back().camp == m_camp)
                return;
        x = (event->x() - 25) / 30;
        y = (event->y() - 25) / 30;
        if(!setPieces(Step(x, y, m_camp)))
            return;
        turn++;
        emit setPiece(1, Step(x, y, m_camp));
        update();
        checkWin();
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
                if(xi - 1 >= 0 && tmp.camp > 0)
                    tmp.lian[2] += (board_[xi-1][yi].camp == tmp.camp?board_[xi-1][yi].lian[2]+1:1);
                else tmp.lian[2] = (tmp.camp > 0);
                if(xi - 1 >= 0 && yi + 1 < m_size && tmp.camp > 0)
                    tmp.lian[0] += (board_[xi-1][yi+1].camp == tmp.camp?board_[xi-1][yi+1].lian[0]+1:1);
                else tmp.lian[0] = (tmp.camp > 0);
                if(yi - 1 >= 0 && tmp.camp > 0)
                    tmp.lian[3] += (board_[xi-1][yi-1].camp == tmp.camp?board_[xi][yi-1].lian[3]+1:1);
                else tmp.lian[3] = (tmp.camp > 0);
                for(int i = 0; i < 4; i++)
                    tmp.maxLian = tmp.maxLian>tmp.lian[i]?tmp.maxLian:tmp.lian[i];
            }
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
    for(int i = 0; i < m_size; ++i)
        for(int j = 0; j < m_size; ++j)
            board_[i][j].clear();
    m_step.clear();
}
