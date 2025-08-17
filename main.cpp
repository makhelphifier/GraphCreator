#include "mainwindow.h"

#include <QApplication>
#include "graphmenubuilder.h"
#include "newgraphwidget.h"
#include "graphtoolbarbuilder.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("绘图工具");
    w.setFixedSize(1200,800);

    GraphMenuBuilder menuBuilder(&w);

    w.setMenuBar(menuBuilder.buildMenuBar());

    w.show();

    // menuBuilder.onNewGraphTriggered(true);

    // menuBuilder.onOpenGraphTriggered(true);
    // menuBuilder.onImportGraphTriggered(true);
    // menuBuilder.onManagerGraphTriggered(true);
    // menuBuilder.onPageSetupGraphTriggered();

    // menuBuilder.on_actionConfigurePreview_triggered();

    NewGraphWidget::writeFileIntoScene("sss",w.m_scene);

    return a.exec();
}
