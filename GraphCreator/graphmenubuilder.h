#ifndef GRAPHMENUBUILDER_H
#define GRAPHMENUBUILDER_H
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QMainWindow>
class GraphMenuBuilder {
public:
    GraphMenuBuilder();

    GraphMenuBuilder(QMainWindow* parent = nullptr);

    QMenuBar* buildMenuBar();
private:
    QMainWindow* m_parentWindow;
    QMenuBar* m_menuBar;
    void createMenus();
    void createGraphActions();


};

#endif // GRAPHMENUBUILDER_H
