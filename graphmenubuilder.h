#ifndef GRAPHMENUBUILDER_H
#define GRAPHMENUBUILDER_H
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QMainWindow>
#include "newgraphwidget.h"

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

private:
    QMainWindow* m_parentWindow;
    QMenuBar* m_menuBar;
    void createMenus();
    void createGraphActions();
    NewGraphWidget* m_newGraphWidget;

};

#endif // GRAPHMENUBUILDER_H
