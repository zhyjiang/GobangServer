#ifndef GOBANGPLAT_H
#define GOBANGPLAT_H

#include "step.h"

#include <QLabel>
#include <QWidget>

class Gobang;

class GobangPlat : public QLabel
{
public:
    GobangPlat(QWidget *parent = 0);
    void reStart();

private:
    void paintEvent(QPaintEvent *);
    bool setPieces(const Step &step);
    int board_[15][15];

    friend class Gobang;
};

#endif // GOBANGPLAT_H
