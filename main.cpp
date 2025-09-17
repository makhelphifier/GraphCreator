#include "mainwindow.h"

#include <QApplication>
#include "graphmenubuilder.h"
#include "newgraphwidget.h"
#include "graphtoolbarbuilder.h"
#include <QTranslator>
#include <QLibraryInfo>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    QString translationsPath = QLibraryInfo::path(QLibraryInfo::TranslationsPath);
    if(translator.load("qt_zh_CN",translationsPath)){
        a.installTranslator(&translator);
    }


    MainWindow w;
    w.setWindowTitle("绘图工具");
    w.setFixedSize(1200,800);

    GraphMenuBuilder menuBuilder(&w);
    QObject::connect(&menuBuilder, &GraphMenuBuilder::openGraph, &w, &MainWindow::loadGraphFromFile);

    w.setMenuBar(menuBuilder.buildMenuBar());

    w.show();
    return a.exec();
}
