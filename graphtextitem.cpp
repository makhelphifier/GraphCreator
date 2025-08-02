#include "graphtextitem.h"

#include <QFont>
#include <QPainter>





GraphTextItem::GraphTextItem(const QString &text, QGraphicsItem *parent)
{

    this->setDefaultTextColor(Qt::black);
    this->setFont(QFont("Arial",12));
    this->setFlags(QGraphicsItem::ItemIsFocusable|QGraphicsItem::ItemIsSelectable|QGraphicsItem::ItemIsMovable);
    this->setPlainText(text);
    this->setTextInteractionFlags(Qt::TextEditorInteraction);
    this->update();
}

void GraphTextItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    drawHandle(painter,QPointF(100,200),8);

    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::NoBrush);
    painter->setBrush(Qt::yellow);


    QRectF boundRect =  this->boundingRect();
    painter->drawRoundedRect(boundRect,5,5);

}


void GraphTextItem::drawHandle(QPainter *painter, const QPointF &pos, qreal size)
{
    qreal halfSize = size /2.0;
    painter->setPen(QPen(Qt::black,1));
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(QRect(pos.x()-halfSize,pos.y()-halfSize,size,size));
    // painter->drawPoint(pos);
}


bool GraphTextItem::sceneEvent(QEvent *event)
{
    return QGraphicsTextItem::sceneEvent(event);
}

void GraphTextItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsTextItem::mousePressEvent(event);
}

void GraphTextItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsTextItem::mouseMoveEvent(event);
}

void GraphTextItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsTextItem::mouseReleaseEvent(event);
}

void GraphTextItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsTextItem::mouseDoubleClickEvent(event);
}

void GraphTextItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    QGraphicsTextItem::contextMenuEvent(event);
}

void GraphTextItem::keyPressEvent(QKeyEvent *event)
{
    QGraphicsTextItem::keyPressEvent(event);
}

void GraphTextItem::keyReleaseEvent(QKeyEvent *event)
{
    QGraphicsTextItem::keyReleaseEvent(event);
}

void GraphTextItem::focusInEvent(QFocusEvent *event)
{
    QGraphicsTextItem::focusInEvent(event);
}

void GraphTextItem::focusOutEvent(QFocusEvent *event)
{
    QGraphicsTextItem::focusOutEvent(event);
}

void GraphTextItem::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    QGraphicsTextItem::dragEnterEvent(event);
}

void GraphTextItem::dragLeaveEvent(QGraphicsSceneDragDropEvent *event)
{
    QGraphicsTextItem::dragLeaveEvent(event);
}

void GraphTextItem::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    QGraphicsTextItem::dragMoveEvent(event);
}

void GraphTextItem::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    QGraphicsTextItem::dropEvent(event);
}

void GraphTextItem::inputMethodEvent(QInputMethodEvent *event)
{
    QGraphicsTextItem::inputMethodEvent(event);
}

void GraphTextItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsTextItem::hoverEnterEvent(event);
}

void GraphTextItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsTextItem::hoverMoveEvent(event);
}

void GraphTextItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsTextItem::hoverLeaveEvent(event);
}
