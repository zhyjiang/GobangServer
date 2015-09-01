#ifndef GOBANG_H
#define GOBANG_H

#include "pieces.h"
#include "step.h"
#include "networkserver.h"

#include <QPaintEvent>
#include <QWidget>

#include <vector>

class MainWindow;

namespace Ui {
class Gobang;
}

class Gobang : public QWidget
{
    Q_OBJECT

public:
    explicit Gobang(QWidget *parent = 0);
    ~Gobang();
    NetworkServer* getNetworkServer() {return &m_server;}

public slots:
    void setHost();
    void reStart();
    bool setPieces(const Step&); // To set a piece

signals:
    void setPiece(int, Step);
    void win(int);

private:
    int turnAdd() {return turn++;}
    int checkWin(); // To check whether there is a winner or not.
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);

    Ui::Gobang *ui;
    Pieces board_[20][20]; // The chessboard.
    NetworkServer m_server;

    std::vector<Step> m_step;

    int turn; // The number of the turns.
    int m_size;
    int m_camp;
    bool m_host;
    enum {white = 1, black = 2};

    friend class MainWindow;
};

#endif // GOBANG_H
