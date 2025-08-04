#include "enhancedlineitem.h"
#include <QPen>
#include <QPainter>
#include <QStyleOptionGraphicsItem>



EnhancedLineItem::EnhancedLineItem() {
    this->setFlags(QGraphicsItem::ItemIsSelectable|QGraphicsItem::ItemIsMovable|QGraphicsItem::ItemIsFocusable);
    QPen pen;
    pen.setWidth(1);
    pen.setColor(Qt::darkBlue);
    pen.setStyle(Qt::SolidLine);
    this->setPen(pen);
    this->update();
}

void EnhancedLineItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QStyleOptionGraphicsItem tempOption(*option);
    // qDebug()<<tempOption.state<<"====before";

    tempOption.state &=~ QStyle::State_Selected;
    tempOption.state &=~ QStyle::State_HasFocus;
    // qDebug()<<tempOption.state<<"====after";
    QGraphicsLineItem::paint(painter,&tempOption,widget);

    if(option->state&QStyle::State_Selected){
        qreal handleSize = 4;
        drawHandle(painter,this->line().p1(),handleSize);
        drawHandle(painter,this->line().p2(),handleSize);

    }
}
void EnhancedLineItem::drawHandle(QPainter *painter, const QPointF &pos, qreal size)
{
    qreal halfSize = size /2.0;
    painter->setPen(QPen(Qt::black,1));
    painter->setBrush(QBrush(Qt::white));
    painter->drawEllipse(pos,size,size);
}
