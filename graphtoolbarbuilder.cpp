#include "graphtoolbarbuilder.h"
#include <QMainWindow>
GraphToolBarBuilder::GraphToolBarBuilder(QMainWindow *parent)

{
    m_parentWindow = parent;
    m_toolBar =new QToolBar(parent);
    createMenus();

}

QToolBar *GraphToolBarBuilder::buildToolBar()
{
    return m_toolBar;
}

void GraphToolBarBuilder::createMenus()
{

    // m_toolBar->addAction("文字",this,&GraphToolBarBuilder::onTextTriggered);
    connect(m_toolBar->addAction("文字"),&QAction::triggered,this,&GraphToolBarBuilder::onTextTriggered);
    connect(m_toolBar->addAction("线段"),&QAction::triggered,this,&GraphToolBarBuilder::onLineTriggered);
    connect(m_toolBar->addAction("多线段"),&QAction::triggered,this,&GraphToolBarBuilder::onPolylineTriggered);
    connect(m_toolBar->addAction("矩形"),&QAction::triggered,this,&GraphToolBarBuilder::onRectangleTriggered);
    connect(m_toolBar->addAction("椭圆"),&QAction::triggered,this,&GraphToolBarBuilder::onEclipseTriggered);
    connect(m_toolBar->addAction("圆弧"),&QAction::triggered,this,&GraphToolBarBuilder::onArcTriggered);
    connect(m_toolBar->addAction("多边形"),&QAction::triggered,this,&GraphToolBarBuilder::onPolygonTriggered);
}

void GraphToolBarBuilder::onTextTriggered()
{
    qDebug()<<"onTextTriggered";
    emit textToolTriggered();
}
void GraphToolBarBuilder::onLineTriggered()
{
    qDebug()<<"onTextTriggered";
    emit lineToolTriggered();
}
void GraphToolBarBuilder::onPolylineTriggered()
{
    qDebug()<<"onPolylineTriggered";
    emit polylineToolTriggered();
}

void GraphToolBarBuilder::onRectangleTriggered()
{
    qDebug()<<"onRectangleTriggered";
    emit rectangleToolTriggered();
}

void GraphToolBarBuilder::onEclipseTriggered()
{
    qDebug()<<"onEclipseTriggered";
    emit eclipseToolTriggered();
}

void GraphToolBarBuilder::onArcTriggered()
{
    qDebug()<<"onArcTriggered";
    emit arcToolTriggered();
}

void GraphToolBarBuilder::onPolygonTriggered()
{
    qDebug()<<"onPolygonTriggered";
    emit polygonToolTriggered();
}
