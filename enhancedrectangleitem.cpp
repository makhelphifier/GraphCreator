#include "enhancedrectangleitem.h"

#include <QPen>
#include <QStyleOptionGraphicsItem>
#include <QPainter>

EnhancedRectangleItem::EnhancedRectangleItem() {
    this->setFlags(QGraphicsItem::ItemIsSelectable|QGraphicsItem::ItemIsMovable|QGraphicsItem::ItemIsFocusable);
    this->setAcceptHoverEvents(true);
    QPen pen;
    pen.setWidth(1);
    pen.setColor(Qt::darkBlue);
    pen.setStyle(Qt::SolidLine);
    this->setPen(pen);
    this->update();
}

QPainterPath EnhancedRectangleItem::shape() const
{

    return QGraphicsRectItem::shape();
}

void EnhancedRectangleItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsRectItem::mousePressEvent(event);
}

void EnhancedRectangleItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsRectItem::mouseMoveEvent(event);

}

void EnhancedRectangleItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsRectItem::mouseReleaseEvent(event);

}

void EnhancedRectangleItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsRectItem::mouseDoubleClickEvent(event);

}

void EnhancedRectangleItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsRectItem::hoverMoveEvent(event);

}

void EnhancedRectangleItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsRectItem::hoverLeaveEvent(event);

}

void EnhancedRectangleItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QStyleOptionGraphicsItem tempOption(*option);
    // qDebug()<<tempOption.state<<"====before";

    tempOption.state &=~ QStyle::State_Selected;
    tempOption.state &=~ QStyle::State_HasFocus;
    // qDebug()<<tempOption.state<<"====after";
    QGraphicsRectItem::paint(painter,&tempOption,widget);

    if(option->state&QStyle::State_Selected){
        QRectF boundRect = this->boundingRect();
        QPen selectionBoxPen(Qt::black,1,Qt::DotLine);
        painter->setPen(selectionBoxPen);
        painter->setBrush(Qt::NoBrush);
        if(!m_isRotating&&m_draggedPointIndex==-1){
            painter->drawRect(boundRect);
            qreal handleSize = 4;
            drawHandle(painter,boundRect.topLeft(),handleSize);
            drawHandle(painter,boundRect.topRight(),handleSize);
            drawHandle(painter,boundRect.bottomLeft(),handleSize);
            drawHandle(painter,boundRect.bottomRight(),handleSize);
            drawHandle(painter,boundRect.center()+QPointF(0,boundRect.height()/2),handleSize);
            drawHandle(painter,boundRect.center()+QPointF(0, -boundRect.height()/2),handleSize);
            drawHandle(painter,boundRect.center()+QPointF(boundRect.width()/2,0),handleSize);
            drawHandle(painter,boundRect.center()+QPointF(-boundRect.width()/2,0),handleSize);
            // for(int i =0;i<this->path().elementCount();i++){
            //     const QPainterPath::Element &element = this->path().elementAt(i);
            //     qreal halfSize = 2 /2.0;
            //     painter->setPen(QPen(Qt::white,0.2));
            //     painter->setBrush(QBrush(Qt::black));
            //     painter->drawEllipse(QRect(element.x-halfSize,element.y-halfSize,2,2));
            // }
            drawRotationHandle(painter);
        }else if(m_isRotating) {
            drawRotationHandle(painter,m_rotationCenter);
        }
    }
}

void EnhancedRectangleItem::drawHandle(QPainter *painter, const QPointF pos, qreal size)
{
    qreal halfSize = size /2.0;
    painter->setPen(QPen(Qt::black,1));
    painter->setBrush(QBrush(Qt::white));
    painter->drawRect(QRect(pos.x()-halfSize,pos.y()-halfSize,size,size));
    // painter->drawPoint(pos);
}



EnhancedRectangleItem::HandlePosition EnhancedRectangleItem::handleAt(const QPointF &pos)
{

    return None;
}

QString EnhancedRectangleItem::handleToString(HandlePosition handle)
{
    return "";
}

QRectF EnhancedRectangleItem::rotationHandleRect() const
{
    return QRectF();
}

void EnhancedRectangleItem::drawRotationHandle(QPainter *painter, QPointF point)
{

}
