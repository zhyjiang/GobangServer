#include "mainwindow.h"
#include <QApplication>
#include <QFontDatabase>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    int fontId = QFontDatabase::addApplicationFont("./Font/Senty.otf");
    QString msyh = QFontDatabase::applicationFontFamilies(fontId).at(0);
    QFont font(msyh);
    QApplication::setFont(font);
    MainWindow w;
    w.show();

    return a.exec();
}
