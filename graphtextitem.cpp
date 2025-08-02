#include "graphtextitem.h"

#include <QFont>
#include <QPainter>





GraphTextItem::GraphTextItem(const QString &text, QGraphicsItem *parent)
{

    this->setDefaultTextColor(Qt::red);
    this->setFont(QFont("Arial",12));
    this->setPlainText(text);

    this->setFlags(QGraphicsItem::ItemIsFocusable|QGraphicsItem::ItemIsSelectable|QGraphicsItem::ItemIsMovable);
    // this->setTextInteractionFlags(Qt::TextEditorInteraction);
    this->update();
}
#include <QStyleOptionGraphicsItem>
void GraphTextItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    drawHandle(painter,QPointF(100,200),8);

    // painter->setPen(Qt::NoPen);
    // painter->setBrush(Qt::NoBrush);
    // // painter->setBrush(Qt::yellow);


    // painter->drawRoundedRect(boundRect,5,5);
    QStyleOptionGraphicsItem tempOption(*option);
    qDebug()<<tempOption.state<<"====before";

    tempOption.state &=~ QStyle::State_Selected;
    tempOption.state &=~ QStyle::State_HasFocus;
    qDebug()<<tempOption.state<<"====after";
    QGraphicsTextItem::paint(painter,&tempOption,widget);

    if(option->state&QStyle::State_Selected){
        QRectF boundRect = this->boundingRect();
        QPen selectionBoxPen(Qt::black,1,Qt::DotLine);
        painter->setPen(selectionBoxPen);
        painter->setBrush(Qt::NoBrush);
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

    }
}


void GraphTextItem::drawHandle(QPainter *painter, const QPointF &pos, qreal size)
{
    qreal halfSize = size /2.0;
    painter->setPen(QPen(Qt::black,1));
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(QRect(pos.x()-halfSize,pos.y()-halfSize,size,size));
    // painter->drawPoint(pos);
}

