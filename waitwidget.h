#ifndef WAITWIDGET_H
#define WAITWIDGET_H

#include <QWidget>

class MainWindow;

namespace Ui {
class waitWidget;
}

class waitWidget : public QWidget
{
    Q_OBJECT

public:
    explicit waitWidget(QWidget *parent = 0);
    ~waitWidget();

private:
    Ui::waitWidget *ui;

    friend class MainWindow;
};

#endif // WAITWIDGET_H
