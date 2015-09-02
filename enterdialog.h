#ifndef ENTERDIALOG_H
#define ENTERDIALOG_H

#include <QDialog>

namespace Ui {
class enterDialog;
}

class enterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit enterDialog(QWidget *parent = 0);
    ~enterDialog();

private:
    Ui::enterDialog *ui;
    QPushButton* button[10];

};

#endif // ENTERDIALOG_H
