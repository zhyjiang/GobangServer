#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "gobang.h"
#include "startmenu.h"
#include "networkserver.h"
#include "waitwidget.h"

#include <QMainWindow>
#include <QStackedWidget>
#include <QStringList>

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
    void setWaiting();
    void findPlayer(QString);
    void refresh();
    void broadcast();
    void connectHost();
    void checkUnactive();
    void setStart();

private:
    Ui::MainWindow *ui;
    Gobang *m_gobang;
    StartMenu *m_startMenu;
    QStackedWidget *m_stack;
    waitWidget *m_waitWidget;
    NetworkServer m_server;
    QString m_name;
    QTimer m_timer;

    QStringList m_ipList, m_nameList;

    QList <int> m_waitList;
};

#endif // MAINWINDOW_H
