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
    fileMenu->addAction("打开(&O)",this,&GraphMenuBuilder::onOpenGraphTriggered);
    fileMenu->addAction("退出(&X)");

}

void GraphMenuBuilder::createGraphActions()
{

}

void GraphMenuBuilder::onNewGraphTriggered(bool checked ){

    qDebug()<<"onNewGraphTriggered ";
    m_newGraphWidget = new NewGraphWidget(m_parentWindow,"NEW");
    m_newGraphWidget->show();

}


void GraphMenuBuilder::onOpenGraphTriggered(bool checked ){

    qDebug()<<"onOpenGraphTriggered ";
    m_newGraphWidget = new NewGraphWidget(m_parentWindow,"OPEN");
    m_newGraphWidget->show();

}


void GraphMenuBuilder::onImportGraphTriggered(bool checked ){

    qDebug()<<"onImportGraphTriggered ";
    m_newGraphWidget = new NewGraphWidget(m_parentWindow,"IMPORT");
    m_newGraphWidget->show();

}


void GraphMenuBuilder::onManagerGraphTriggered(bool checked ){

    qDebug()<<"onManagerGraphTriggered ";
    m_newGraphWidget = new NewGraphWidget(m_parentWindow,"MANAGER");
    m_newGraphWidget->show();

}
