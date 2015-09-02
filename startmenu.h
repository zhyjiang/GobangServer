#ifndef STARTMENU_H
#define STARTMENU_H

#include <QWidget>
#include <QListWidgetItem>

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

private slots:
    void activeItem(QListWidgetItem*);

private:
    Ui::StartMenu *ui;
    QString currentIp;

    friend class MainWindow;
};

#endif // STARTMENU_H
