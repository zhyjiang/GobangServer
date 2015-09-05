#ifndef WINWIDGET_H
#define WINWIDGET_H

#include <QWidget>
#include <QGraphicsOpacityEffect>
#include <QTimer>

class Gobang;

namespace Ui {
class WinWidget;
}

class WinWidget : public QWidget
{
    Q_OBJECT

public:
    explicit WinWidget(int _type, QWidget *parent = 0);
    ~WinWidget();
    enum {win, save, load, recall, disconnect, waitForRecall, waitForExit, exit};

private slots:
    void saveGame();
    void agree();
    void thicker();
    void thinner();
    void isAgreeExit();
    void refuse();

signals:
    void saveFile(QString);
    void isClosed(bool);
    void agreeRecall();
    void agreeExit();
    void haveRefused();

private:
    void paintEvent(QPaintEvent *);
    Ui::WinWidget *ui;
    QGraphicsOpacityEffect m_effect;
    QTimer time;

    double m_opacity;

    friend class Gobang;
};

#endif // WINWIDGET_H
