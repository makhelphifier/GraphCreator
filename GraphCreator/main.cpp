#include "mainwindow.h"

#include <QApplication>
#include "graphmenubuilder.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("绘图工具");
    w.setFixedSize(1200,800);

    GraphMenuBuilder menuBuilder(&w);
    w.setMenuBar(menuBuilder.buildMenuBar());
    w.show();

    menuBuilder.onNewGraphTriggered();

    return a.exec();
}
