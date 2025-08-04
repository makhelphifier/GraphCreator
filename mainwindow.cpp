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
    // // 例如，添加一个矩形来测试
    QGraphicsRectItem* rectItem = new QGraphicsRectItem(0, 0, 100, 100);
    rectItem->setBrush(Qt::green);
    m_scene->addItem(rectItem);
    setCentralWidget(m_view);


}

MainWindow::~MainWindow() {

}
