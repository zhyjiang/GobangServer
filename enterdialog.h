#ifndef ENTERDIALOG_H
#define ENTERDIALOG_H

#include <QDialog>
#include <QString>
#include <QSignalMapper>

namespace Ui {
class enterDialog;
}

class enterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit enterDialog(QWidget *parent = 0);
    ~enterDialog();

private slots:
    void display(int);
    void on_ok_clicked();

signals:
    void setIP(QString);

private:
    Ui::enterDialog *ui;
    QPushButton* button[10];
    QPushButton* del, *dot;
    QSignalMapper m_map;
};

#endif // ENTERDIALOG_H
