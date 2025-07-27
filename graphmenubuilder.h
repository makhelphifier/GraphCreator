#ifndef GRAPHMENUBUILDER_H
#define GRAPHMENUBUILDER_H
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QMainWindow>
#include "newgraphwidget.h"
#include <QGraphicsScene>
#include "pagesetupdialog.h"

class GraphMenuBuilder :public QObject {
    Q_OBJECT
public:
    GraphMenuBuilder();

    GraphMenuBuilder(QMainWindow* parent = nullptr);

    QMenuBar* buildMenuBar();
public slots:
    void onNewGraphTriggered(bool checked = false);
    void onOpenGraphTriggered(bool checked);
    void onImportGraphTriggered(bool checked);
    void onManagerGraphTriggered(bool checked);
    void onExportGraphTriggered(bool checked);
    void onPrintGraphTriggered(bool checked);
    void printGraph();
    void onPageSetupGraphTriggered();
private:
    QMainWindow* m_parentWindow;
    QMenuBar* m_menuBar;
    QGraphicsScene* m_currentScene;
    void createMenus();
    void createGraphActions();
    NewGraphWidget* m_newGraphWidget;
    PageSetupDialog* m_pageSetupDialog;
};

#endif // GRAPHMENUBUILDER_H
