#include "enterdialog.h"
#include "ui_enterdialog.h"

enterDialog::enterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::enterDialog)
{
    ui->setupUi(this);
}

enterDialog::~enterDialog()
{
    delete ui;
}
