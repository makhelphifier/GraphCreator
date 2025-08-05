#include "mainwindow.h"

#include <QGraphicsRectItem>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    m_scene = new QGraphicsScene(this);
    m_view = new QGraphicsView(m_scene,this);
    m_view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    m_view->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    m_scene->setSceneRect(0, 0, 1584, 768); // 设置场景的推荐大小
    setCentralWidget(m_view);
    m_view->setDragMode(QGraphicsView::RubberBandDrag); // 允许框选
    m_view->scale(4,4);
    m_view->setRenderHint(QPainter::Antialiasing,true);

}

MainWindow::~MainWindow() {

}
#include <QWheelEvent>
void MainWindow::wheelEvent(QWheelEvent *event)
{
    if(event->modifiers()&Qt::ControlModifier){
        if(event->angleDelta().y()>0){
            m_view->scale(1.2,1.2);
        }else{
            m_view->scale(1/1.2,1/1.2);

        }
    }else{
        QMainWindow::wheelEvent(event);
    }
}
