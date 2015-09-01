#ifndef STARTMENU_H
#define STARTMENU_H

#include <QWidget>

class MainWindow;

namespace Ui {
class StartMenu;
}

class StartMenu : public QWidget
{
    Q_OBJECT

public:
    explicit StartMenu(QWidget *parent = 0);
    ~StartMenu();

private:
    Ui::StartMenu *ui;

    friend class MainWindow;
};

#endif // STARTMENU_H
