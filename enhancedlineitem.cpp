#include "enhancedlineitem.h"
#include <QPen>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QLineF>
#include <QTransform>

#include <QDebug>

EnhancedLineItem::EnhancedLineItem() {
    this->setFlags(QGraphicsItem::ItemIsSelectable|QGraphicsItem::ItemIsMovable|QGraphicsItem::ItemIsFocusable);
    this->setAcceptHoverEvents(true);
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
        const qreal lod = QStyleOptionGraphicsItem::levelOfDetailFromTransform(painter->worldTransform());
        const qreal handlePixelSize = 8;
        const qreal handleSceneSize = handlePixelSize / lod ;
        drawHandle(painter,this->line().p1(),handleSceneSize);
        drawHandle(painter,this->line().p2(),handleSceneSize);
        drawRotationHandle(painter);
    }
}

void EnhancedLineItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "--- mousePressEvent ---";
    bool isOnHandle = isPostionOnRotationHandle(event->pos());
    qDebug() << "Is on rotation handle?" << isOnHandle;

    m_activeHandle = handleAt(event->pos());
    if(m_activeHandle!=NoHandle){
        if(m_activeHandle == RotationHandle){
            m_isRotating = true;
            setCursor(Qt::CrossCursor);
            event->accept();
            // qDebug() << "Rotation mode activated. Event accepted. Returning.";
            return;
        }else{
            setCursor(Qt::CrossCursor);
            event->accept();
            // qDebug() << "Dragging mode activated. Event accepted. Returning.";
            return;
        }
    }
    qDebug() << "Not on handle. Passing event to base class.";
    m_isRotating = false;
    QGraphicsLineItem::mousePressEvent(event);
}

void EnhancedLineItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(m_activeHandle == StartHandle ){
        this->setLine(QLineF(event->pos(),this->line().p2()));
    }else if(m_activeHandle == EndHandle){
        this->setLine(QLineF(this->line().p1(),event->pos()));
    }else if(m_isRotating){
        QPointF center = (this->line().p1()+this->line().p2())/2;
        QLineF lineToLastPos(center,event->lastPos());
        QLineF lineToCurrentPos(center,event->pos());
        qreal angelDelta = lineToLastPos.angleTo(lineToCurrentPos);

        QPointF p1 = this->line().p1();
        QPointF p2 = this->line().p2();

        QTransform transform ;
        transform.translate(center.x(),center.y());  // 1. 将坐标系原点移动到旋转中心
        transform.rotate(-angelDelta);               // 2. 旋转（Qt的angle是逆时针为正，所以用负号）
        transform.translate(-center.x(),-center.y());// 3. 将坐标系原点移回
        QPointF newP1 = transform.map(p1);
        QPointF newP2 = transform.map(p2);
        this->setLine(QLineF(newP1,newP2));
        event->accept();

    }else{
        QGraphicsLineItem::mouseMoveEvent(event);
    }
}

void EnhancedLineItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(m_isRotating){
        m_isRotating=false;
        unsetCursor();
        event->accept();
    }else{
        QGraphicsLineItem::mouseReleaseEvent(event);
    }
}

QPainterPath EnhancedLineItem::shape() const
{
    QPainterPath path;
    QPainterPathStroker stroker ;
    stroker.setWidth(10);
    stroker.setCapStyle(Qt::FlatCap);

    QPainterPath linePath;
    linePath.moveTo(this->line().p1());
    linePath.lineTo(this->line().p2());

    path.addPath(stroker.createStroke(linePath));
    const qreal handlePixelSize = 8;
    path.addEllipse(this->line().p1(),handlePixelSize,handlePixelSize);
    path.addEllipse(this->line().p2(),handlePixelSize,handlePixelSize);

    QRectF rotHandleRect = rotationHandleRect();
    path.addEllipse(rotHandleRect);
    return path;
}

void EnhancedLineItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    Handle handle = handleAt(event->pos());
    if(handle!=NoHandle){
        this->setCursor(Qt::CrossCursor);
    }else{
        this->setCursor(Qt::ArrowCursor);
    }
    QGraphicsLineItem::hoverMoveEvent(event);
}

void EnhancedLineItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    this->setCursor(Qt::ArrowCursor);
    QGraphicsLineItem::hoverLeaveEvent(event);
}
void EnhancedLineItem::drawHandle(QPainter *painter, const QPointF &pos, qreal size)
{
    qreal halfSize = size /2.0;
    QPen pen(Qt::black,1);
    // pen.setCosmetic(true);
    painter->setPen(pen);
    painter->setBrush(QBrush(Qt::white));
    painter->drawEllipse(pos,halfSize,halfSize);
}


void EnhancedLineItem::drawRotationHandle(QPainter *painter)
{
    QRectF handleRect = rotationHandleRect();
    const qreal lod = QStyleOptionGraphicsItem::levelOfDetailFromTransform(painter->worldTransform());
    qreal ellipseRadius = handleRect.width() / 2.0 / lod;
    QPen pen(Qt::black,1);
    pen.setCosmetic(true);
    painter->setPen(pen);
    painter->setBrush(Qt::blue);
    painter->drawEllipse(handleRect.center(),ellipseRadius,ellipseRadius);
}

QRectF EnhancedLineItem::rotationHandleRect() const
{
    QPointF start = this->line().p1();
    QPointF end = this->line().p2();
    QPointF middle = (start + end) / 2.0;
    QPointF direction = end - start;
    QPointF normalVector = QPointF(-direction.y(), direction.x());
    qreal length = std::sqrt(QPointF::dotProduct(normalVector, normalVector));
    if (length > 0) {
        normalVector /= length;
    }
    const qreal desirePixelOffset = 20.0;
    QPointF targetPoint = middle + normalVector * desirePixelOffset;
    const qreal handlePixelSize = 8.0;
    return QRectF(targetPoint - QPointF(handlePixelSize / 2, handlePixelSize / 2), QSizeF(handlePixelSize, handlePixelSize));

}

bool EnhancedLineItem::isPostionOnRotationHandle(const QPointF &pos) const
{
    return rotationHandleRect().contains(pos);
}

EnhancedLineItem::Handle EnhancedLineItem::handleAt(const QPointF &pos) const
{
    const qreal handleRadius =8.0;
    // qDebug()<<"start length === "<< QLineF(this->line().p1(),pos).length();
    // qDebug()<<"???==="<<(QLineF(this->line().p1(),pos).length()<=handleRadius);
    if(QLineF(this->line().p1(),pos).length()<=handleRadius){
        return StartHandle;
    }else if(QLineF(this->line().p2(),pos).length()<=handleRadius){
        return EndHandle;
    }else if(rotationHandleRect().contains(pos)){
        return RotationHandle;
    }else{
        return NoHandle;
    }
}
