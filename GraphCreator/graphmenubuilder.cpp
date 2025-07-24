#include "graphmenubuilder.h"

GraphMenuBuilder::GraphMenuBuilder() {}

GraphMenuBuilder::GraphMenuBuilder(QMainWindow *parent)
{

    m_menuBar = new QMenuBar();
    createMenus();
    createGraphActions();
}

QMenuBar *GraphMenuBuilder::buildMenuBar()
{

    return m_menuBar;
}

void GraphMenuBuilder::createMenus()
{
    QMenu* fileMenu = m_menuBar->addMenu("文件(&F)");
    fileMenu->addAction("新建(&N)",this,&GraphMenuBuilder::onNewGraphTriggered);
    fileMenu->addAction("打开(&O)");
    fileMenu->addAction("退出(&X)");

}

void GraphMenuBuilder::createGraphActions()
{

}

void GraphMenuBuilder::onNewGraphTriggered(bool checked ){

    qDebug()<<"onNewGraphTriggered ";
    m_newGraphWidget = new NewGraphWidget(m_parentWindow);
    m_newGraphWidget->show();

}
