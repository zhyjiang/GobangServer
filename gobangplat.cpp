#include "gobangplat.h"

#include <QBrush>
#include <QPainter>
#include <QPen>

GobangPlat::GobangPlat(QWidget *parent) :
    QLabel(parent)
{
    resize(QSize(460, 462));
    this->setScaledContents(true);

    for(int i = 0; i < 15; i++)
        for(int j = 0; j < 15; j++)
            board_[i][j] = 0;
}

void GobangPlat::reStart()
{
    for(int i = 0; i < 15; i++)
        for(int j = 0; j < 15; j++)
            board_[i][j] = 0;
}

void GobangPlat::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);

    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    for (int i = 0; i < 15; i++)
        for (int j = 0; j < 15; j++)
        {
            if (board_[i][j] == 2)
            {
                brush.setColor(Qt::black);
                p.setBrush(brush);
                p.setPen(QPen(Qt::black));
                p.drawEllipse(QPoint(20 + i * 30, 20 + j * 30), 12, 12);
            }
            else if (board_[i][j] == 1)
            {
                brush.setColor(Qt::white);
                p.setBrush(brush);
                p.setPen(QPen(Qt::white));
                p.drawEllipse(QPoint(20 + i * 30, 20 + j * 30), 12, 12);
            }
        }
}

bool GobangPlat::setPieces(const Step &step)
{
    board_[step.x][step.y] = step.camp;
    return true;
}
