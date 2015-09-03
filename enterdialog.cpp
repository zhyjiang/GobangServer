#include "enterdialog.h"
#include "ui_enterdialog.h"

enterDialog::enterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::enterDialog)
{
    ui->setupUi(this);
    for(int i = 0; i < 10; ++i)
    {
        button[i] = new QPushButton(QString::number(i), this);
        connect(button[i], SIGNAL(clicked()), &m_map, SLOT(map()));
        m_map.setMapping(button[i], i);
    }
    del = new QPushButton("del", this);
    connect(del, SIGNAL(clicked()), &m_map, SLOT(map()));
    m_map.setMapping(del, 11);
    dot = new QPushButton(".", this);
    connect(dot, SIGNAL(clicked()), &m_map, SLOT(map()));
    m_map.setMapping(dot, 10);
    connect(&m_map, SIGNAL(mapped(int)), this, SLOT(display(int)));
    for(int i = 0; i < 10; ++i)
        ui->gridLayout->addWidget(button[i], i/7, i>6?i-6:i);
    ui->gridLayout->addWidget(dot, 1, 4);
    ui->gridLayout->addWidget(del, 1, 5);
}

enterDialog::~enterDialog()
{
    delete ui;
}

void enterDialog::display(int num)
{
    if(num < 10)
    {
        QString temp = ui->lineEdit->text();
        temp.append(QString::number(num));
        ui->lineEdit->setText(temp);
    }
    else if(num == 10)
    {
        QString temp = ui->lineEdit->text();
        temp.append(".");
        ui->lineEdit->setText(temp);
    }
    else if(num == 11)
        ui->lineEdit->del();
}

void enterDialog::on_ok_clicked()
{
    emit setIP(ui->lineEdit->text());
    close();
}
