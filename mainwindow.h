#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "gobang.h"
#include "startmenu.h"

#include <QMainWindow>
#include <QStackedWidget>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void setGobang();

private:
    Ui::MainWindow *ui;
    Gobang *m_gobang;
    StartMenu *m_startMenu;
    QStackedWidget *m_stack;
};

#endif // MAINWINDOW_H
