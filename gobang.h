#ifndef GOBANG_H
#define GOBANG_H

#include "pieces.h"
#include "step.h"
#include "networkserver.h"
#include "gobangplat.h"

#include <QPaintEvent>
#include <QWidget>
#include <QTimer>
#include <QSound>

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

public slots:
    void setHost();
    void reStart();
    bool setPieces(const Step&); // To set a piece
    void setWin(int camp);
    void timeOut();
    void changeCamp(int camp);
    void recallDone(int);
    void saveGame(QString);
    void on_recall();
    void forRecall();
    void forExit();
    void agreeRecall();
    void agreeExit();
    void changePress() {m_noPress = false;}
    void on_back_click();
    void on_exit() {emit haveExited();}
    void refuse() {emit sendSignal(11, Step(0,0,0));}
    void on_refuse() {emit isRefused();}

signals:
    void haveAgreed();
    void sendSignal(int, Step);
    void win(int);
    void haveExited();
    void isRefused();

private slots:
    void on_recall_clicked();
    void changeCurrentState(int, int, int);
    void on_save_clicked();
    void on_load_clicked();

private:
    int turnAdd() {return turn++;}
    int checkWin(); // To check whether there is a winner or not.
    void mousePressEvent(QMouseEvent *);

    Ui::Gobang *ui;
    Pieces board_[20][20]; // The chessboard.
    QTimer m_timer;
    GobangPlat m_plat;
    QSound m_setPiece, m_win, m_lose;

    std::vector<Step> m_step;

    int turn; // The number of the turns.
    int m_size;
    int m_camp;
    int wbStep[2];
    int currentCamp;
    int wbTime[2];
    int timeCount;
    bool m_noPress;
    bool m_host;
    enum {white = 1, black = 2};

    friend class MainWindow;
};

#endif // GOBANG_H
