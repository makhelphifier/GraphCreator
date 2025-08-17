#include "mainwindow.h"
#include <QWheelEvent>
#include <QPainterPath>

#include <QGraphicsRectItem>
#include "enhancedtextitem.h"
#include "enhancedlineitem.h"
#include "graphtoolbarbuilder.h"
#include "enhancedellipseitem.h"
#include "enhancedarcitem.h"
#include "enhancedpieitem.h"
#include "enhancedpolygonitem.h"
#include "enhancedrectangleitem.h"
#include "enhancedpolylineitem.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    m_scene = new QGraphicsScene(this);
    m_view = new QGraphicsView(m_scene,this);
    m_view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    m_view->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    m_view->viewport()->setMouseTracking(true);
    m_scene->setSceneRect(0, 0, 1584, 768); // 设置场景的推荐大小
    setCentralWidget(m_view);
    m_view->setDragMode(QGraphicsView::RubberBandDrag); // 允许框选
    m_view->scale(4,4);
    m_view->setRenderHint(QPainter::Antialiasing,true);
    // m_view->installEventFilter(this);
    m_view->viewport()->installEventFilter(this);
    GraphToolBarBuilder* toolBarBuilder = new GraphToolBarBuilder(this);
    this->addToolBar(toolBarBuilder->buildToolBar());
    connect(toolBarBuilder,&GraphToolBarBuilder::textToolTriggered,this,&MainWindow::enterTextDrawingMode);
    connect(toolBarBuilder,&GraphToolBarBuilder::lineToolTriggered,this,&MainWindow::enterLineDrawingMode);
    connect(toolBarBuilder,&GraphToolBarBuilder::polylineToolTriggered,this,&MainWindow::enterPolylineDrawingMode);
    connect(toolBarBuilder,&GraphToolBarBuilder::rectangleToolTriggered,this,&MainWindow::enterRectangleDrawingMode);
    connect(toolBarBuilder,&GraphToolBarBuilder::eclipseToolTriggered,this,&MainWindow::enterEllipseDrawingMode);
    connect(toolBarBuilder,&GraphToolBarBuilder::arcToolTriggered,this,&MainWindow::enterArcDrawingMode);
    connect(toolBarBuilder,&GraphToolBarBuilder::polygonToolTriggered,this,&MainWindow::enterPolygonDrawingMode);
}

MainWindow::~MainWindow() {

}



void MainWindow::wheelEvent(QWheelEvent *event)
{
    if(event->modifiers()&Qt::ControlModifier){
        m_view->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
        if(event->angleDelta().y()>0){
            m_view->scale(1.2,1.2);
        }else{
            m_view->scale(1/1.2,1/1.2);
        }
        m_view->setTransformationAnchor(QGraphicsView::AnchorViewCenter);
    }else{
        QMainWindow::wheelEvent(event);
    }
}
void MainWindow::enterTextDrawingMode()
{
    qDebug()<<"enterTextDrawingMode";
    m_currentMode = DrawText;
    m_view->viewport()->setCursor(Qt::CrossCursor);
    m_view->setDragMode(QGraphicsView::NoDrag);
}

void MainWindow::enterLineDrawingMode()
{
    qDebug()<<"enterLineDrawingMode";
    m_currentMode = DrawLine;
    m_view->viewport()->setCursor(Qt::CrossCursor);
    m_view->setDragMode(QGraphicsView::NoDrag);
}

void MainWindow::enterPolylineDrawingMode()
{
    qDebug()<<"enterPolylineDrawingMode";
    m_currentMode = DrawPolyline;
    m_view->viewport()->setCursor(Qt::CrossCursor);
    m_view->setDragMode(QGraphicsView::NoDrag);
}

void MainWindow::enterRectangleDrawingMode()
{
    qDebug()<<"enterRectangleDrawingMode";
    m_currentMode = DrawRectangle;
    m_view->viewport()->setCursor(Qt::CrossCursor);
    m_view->setDragMode(QGraphicsView::NoDrag);
}

void MainWindow::enterEllipseDrawingMode()
{
    qDebug()<<"enterEllipseDrawingMode";
    m_currentMode = DrawEllipse;
    m_view->viewport()->setCursor(Qt::CrossCursor);
    m_view->setDragMode(QGraphicsView::NoDrag);
}

void MainWindow::enterArcDrawingMode()
{
    qDebug()<<"enterArcDrawingMode";
    m_currentMode = DrawArc;
    m_view->viewport()->setCursor(Qt::CrossCursor);
    m_view->setDragMode(QGraphicsView::NoDrag);
}

void MainWindow::enterPolygonDrawingMode()
{
    qDebug()<<"enterPolygonDrawingMode";
    m_currentMode = DrawPolygon;
    m_view->viewport()->setCursor(Qt::CrossCursor);
    m_view->setDragMode(QGraphicsView::NoDrag);
}

bool MainWindow::eventFilter(QObject* watched,QEvent* event){
    if(watched == m_view->viewport()){
        if(m_currentMode == DrawText){
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
            if(event->type() == QEvent::MouseButtonPress && mouseEvent->button() == Qt::LeftButton){
                QPointF scenePos = m_view->mapToScene(mouseEvent->pos());
                if(m_currentItem == nullptr){
                    m_startPoint = scenePos;
                    m_currentItem = new EnhancedTextItem("测试文本");
                    m_currentItem->setPos(m_startPoint);
                    m_scene->addItem(m_currentItem);
                    qDebug()<<"第一次点击";
                }else{
                    m_currentItem->setTextInteractionFlags(Qt::TextEditorInteraction);
                    m_currentItem = nullptr;
                    m_currentMode = NoMode;
                    m_view->setDragMode(QGraphicsView::RubberBandDrag);
                    m_view->setCursor(Qt::ArrowCursor);
                    qDebug()<<"第二次点击";
                }
                return true;
            }else if(event->type() == QEvent::MouseMove) {
                if(m_currentItem!=nullptr){
                    QPointF scenePos = m_view->mapToScene(mouseEvent->pos());
                    qreal width = abs(scenePos.x()-m_startPoint.x());
                    qreal height = abs(scenePos.y()-m_startPoint.y());
                    m_currentItem->setTextWidth(width);
                    qreal left = qMin(scenePos.x(),m_startPoint.x());
                    qreal top = qMin(scenePos.y(),m_startPoint.y());
                    m_currentItem->setPos(left,top);
                }
                return true;
            }
        }else  if(m_currentMode == DrawLine){
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
            if(event->type() == QEvent::MouseButtonPress && mouseEvent->button() == Qt::LeftButton){
                qDebug() << "控件坐标:" << mouseEvent->pos() << "场景坐标:" << m_view->mapToScene(mouseEvent->pos());
                QPointF scenePos = m_view->mapToScene(mouseEvent->pos());
                if(m_currentLineItem == nullptr){
                    m_startPoint = scenePos;
                    m_currentLineItem = new EnhancedLineItem();
                    // m_currentLineItem->setPos(m_startPoint);
                    m_currentLineItem->setLine(QLineF( m_startPoint,m_startPoint));
                    m_currentLineItem->setVisible(false);
                    m_scene->addItem(m_currentLineItem);
                    qDebug()<<"第一次点击";
                }else{
                    m_currentLineItem->setVisible(true);
                    m_currentLineItem->setLine(QLineF(m_startPoint,scenePos));
                    m_currentLineItem = nullptr;
                    m_currentMode = NoMode;
                    m_view->setDragMode(QGraphicsView::RubberBandDrag);
                    // m_view->setCursor(Qt::ArrowCursor);
                    m_view->viewport()->setCursor(Qt::ArrowCursor);
                    qDebug()<<"第二次点击";
                }
                return true;
            }else if(event->type() == QEvent::MouseMove) {
                if(m_currentLineItem!=nullptr){
                    qDebug()<<"第一次点击后，鼠标move移动";
                    QPointF scenePos = m_view->mapToScene(mouseEvent->pos());
                    m_currentLineItem->setLine(QLineF(m_startPoint, scenePos));
                    m_currentLineItem->setVisible(true);

                }
                return true;
            }
        }else if (m_currentMode == DrawPolyline) {
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
            if (event->type() == QEvent::MouseButtonPress && mouseEvent->button() == Qt::LeftButton) {
                QPointF scenePos = m_view->mapToScene(mouseEvent->pos());
                if (m_currentPolylineItem == nullptr) {
                    // 第一次点击：创建路径和item
                    m_polylinePath = new QPainterPath(scenePos); // 直接用起始点构造路径
                    m_currentPolylineItem = new EnhancedPolylineItem();
                    m_currentPolylineItem->setPath(*m_polylinePath);
                    m_scene->addItem(m_currentPolylineItem);
                    qDebug() << "第一次点击";
                } else {
                    // 后续点击：在路径中添加一个确定的点
                    m_polylinePath->lineTo(scenePos);
                    m_currentPolylineItem->setPath(*m_polylinePath);
                    qDebug() << "后续点击，添加点";
                }
                return true;
            } else if (event->type() == QEvent::MouseMove) {
                if (m_currentPolylineItem != nullptr) {
                    // 鼠标移动：绘制预览线
                    QPointF scenePos = m_view->mapToScene(mouseEvent->pos());
                    QPainterPath previewPath = *m_polylinePath; // 复制当前已确定的路径
                    previewPath.lineTo(scenePos); // 添加到当前鼠标位置的临时线段
                    m_currentPolylineItem->setPath(previewPath); // 更新Item显示预览路径
                }
                return true;
            } else if (event->type() == QEvent::MouseButtonDblClick) {
                qDebug() << "鼠标双击，结束绘制";
                if(m_currentPolylineItem != nullptr) {
                    m_currentPolylineItem->setPath(*m_polylinePath);
                }
                m_currentPolylineItem = nullptr;
                delete m_polylinePath;
                m_polylinePath = nullptr;
                m_currentMode = NoMode;
                m_view->setDragMode(QGraphicsView::RubberBandDrag);
                m_view->viewport()->setCursor(Qt::ArrowCursor);
                return true;
            }
        }
        else  if(m_currentMode == DrawRectangle){
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
            if(event->type() == QEvent::MouseButtonPress && mouseEvent->button() == Qt::LeftButton){
                qDebug() << "控件坐标:" << mouseEvent->pos() << "场景坐标:" << m_view->mapToScene(mouseEvent->pos());
                QPointF scenePos = m_view->mapToScene(mouseEvent->pos());
                if(m_currentRectangleItem == nullptr){
                    m_startPoint = scenePos;
                    m_currentRectangleItem = new EnhancedRectangleItem();
                    // m_currentLineItem->setPos(m_startPoint);
                    m_currentRectangleItem->setRect(QRectF( m_startPoint,m_startPoint));
                    m_currentRectangleItem->setVisible(false);
                    m_scene->addItem(m_currentRectangleItem);
                    qDebug()<<"第一次点击";
                }else{
                    m_currentRectangleItem->setVisible(true);
                    m_currentRectangleItem->setRect(QRectF(m_startPoint,scenePos));
                    m_currentRectangleItem = nullptr;
                    m_currentMode = NoMode;
                    m_view->setDragMode(QGraphicsView::RubberBandDrag);
                    // m_view->setCursor(Qt::ArrowCursor);
                    m_view->viewport()->setCursor(Qt::ArrowCursor);
                    qDebug()<<"第二次点击";
                }
                return true;
            }else if(event->type() == QEvent::MouseMove) {
                if(m_currentRectangleItem!=nullptr){
                    qDebug()<<"第一次点击后，鼠标move移动";
                    QPointF scenePos = m_view->mapToScene(mouseEvent->pos());
                    m_currentRectangleItem->setRect(QRectF(m_startPoint, scenePos));
                    m_currentRectangleItem->setVisible(true);
                }
                return true;
            }
        }
        else  if(m_currentMode == DrawEllipse){
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
            if(event->type() == QEvent::MouseButtonPress && mouseEvent->button() == Qt::LeftButton){
                qDebug() << "控件坐标:" << mouseEvent->pos() << "场景坐标:" << m_view->mapToScene(mouseEvent->pos());
                QPointF scenePos = m_view->mapToScene(mouseEvent->pos());
                if(m_currentEllipseItem == nullptr){
                    m_startPoint = scenePos;
                    m_currentEllipseItem = new EnhancedEllipseItem();
                    // m_currentLineItem->setPos(m_startPoint);
                    m_currentEllipseItem->setRect(QRectF( m_startPoint,m_startPoint));
                    m_currentEllipseItem->setVisible(false);
                    m_scene->addItem(m_currentEllipseItem);
                    qDebug()<<"第一次点击";
                }else{
                    m_currentEllipseItem->setVisible(true);
                    m_currentEllipseItem->setRect(QRectF(m_startPoint,scenePos));
                    m_currentEllipseItem = nullptr;
                    m_currentMode = NoMode;
                    m_view->setDragMode(QGraphicsView::RubberBandDrag);
                    // m_view->setCursor(Qt::ArrowCursor);
                    m_view->viewport()->setCursor(Qt::ArrowCursor);
                    qDebug()<<"第二次点击";
                }
                return true;
            }else if(event->type() == QEvent::MouseMove) {
                if(m_currentEllipseItem!=nullptr){
                    qDebug()<<"第一次点击后，鼠标move移动";
                    QPointF scenePos = m_view->mapToScene(mouseEvent->pos());
                    m_currentEllipseItem->setRect(QRectF(m_startPoint, scenePos));
                    m_currentEllipseItem->setVisible(true);
                }
                return true;
            }
        }
        else  if(m_currentMode == DrawArc){
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
            if(event->type() == QEvent::MouseButtonPress && mouseEvent->button() == Qt::LeftButton){
                qDebug() << "控件坐标:" << mouseEvent->pos() << "场景坐标:" << m_view->mapToScene(mouseEvent->pos());
                QPointF scenePos = m_view->mapToScene(mouseEvent->pos());
                if(m_currentLineItem == nullptr && m_currentArcItem ==nullptr ){
                    m_startPoint = scenePos;
                    m_currentLineItem = new EnhancedLineItem();
                    // m_currentLineItem->setPos(m_startPoint);
                    m_currentLineItem->setLine(QLineF( m_startPoint,m_startPoint));
                    QPen pen = m_currentLineItem->pen();
                    pen.setStyle(Qt::DotLine);
                    pen.setWidth(1);
                    m_currentLineItem->setPen(pen);
                    m_currentLineItem->setVisible(false);
                    m_scene->addItem(m_currentLineItem);
                    qDebug()<<"第一次点击";
                }else if(m_currentLineItem != nullptr && m_currentArcItem ==nullptr) {
                    m_currentLineItem->setVisible(true);
                    m_currentLineItem->setLine(QLineF(m_startPoint,scenePos));
                    m_currentArcItem = new EnhancedArcItem();
                    m_currentArcItem->setVisible(false);
                    m_scene->addItem(m_currentArcItem);
                    qDebug()<<"第二次点击";
                }else if(m_currentLineItem != nullptr && m_currentArcItem !=nullptr){
                    m_currentLineItem->setVisible(true);
                    m_currentArcItem->setVisible(false);
                    m_currentArcItem = new EnhancedArcItem(m_currentLineItem->line().p1(),m_currentLineItem->line().p2(),scenePos);

                    m_currentLineItem = nullptr;
                    m_currentArcItem = nullptr;
                    m_currentMode = NoMode;
                    m_view->setDragMode(QGraphicsView::RubberBandDrag);
                    m_view->viewport()->setCursor(Qt::ArrowCursor);
                    qDebug()<<"第三次点击";
                }
                return true;
            }else if(event->type() == QEvent::MouseMove) {
                if(m_currentLineItem!=nullptr && m_currentArcItem ==nullptr){
                    // qDebug()<<"第一次点击后，鼠标move移动";
                    QPointF scenePos = m_view->mapToScene(mouseEvent->pos());
                    m_currentLineItem->setLine(QLineF(m_startPoint, scenePos));
                    m_currentLineItem->setVisible(true);
                }else if(m_currentLineItem!=nullptr && m_currentArcItem !=nullptr){
                    qDebug()<<"第二次点击后，鼠标move移动";
                    QPointF scenePos = m_view->mapToScene(mouseEvent->pos());
                    m_currentArcItem = new EnhancedArcItem(m_currentLineItem->line().p1(),m_currentLineItem->line().p2(),scenePos);
                    m_currentArcItem->setVisible(false);
                    m_currentLineItem->setVisible(true);
                }
                return true;
            }
        }
    }
    return QMainWindow::eventFilter(watched, event);
}



void MainWindow::removeLastPointFromPath(QPainterPath &path)
{
    if(path.isEmpty())  return;

    int elementCount = path.elementCount();
    // if(elementCount<=1){
    //     path =*( new QPainterPath());
    //     return;
    // }

    QPainterPath newPath;

    for(int i =0;i<elementCount-1;i++){
        const QPainterPath::Element &element = path.elementAt(i);
        switch (element.type){
        case QPainterPath::MoveToElement:
            newPath.moveTo(element);
            break;
        case QPainterPath::LineToElement:
            newPath.lineTo(element);
            break;
        case  QPainterPath::CurveToDataElement:
        case QPainterPath::CurveToElement:
            break;
        }
    }
    path = newPath;
}
