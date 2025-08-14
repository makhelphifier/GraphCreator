#include "enhancedellipseitem.h"
#include <QRectF>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneHoverEvent>


EnhancedEllipseItem::EnhancedEllipseItem() {
    this->setFlags(QGraphicsItem::ItemIsSelectable|QGraphicsItem::ItemIsMovable|QGraphicsItem::ItemIsFocusable);
    this->setAcceptHoverEvents(true);
    QPen pen;
    pen.setWidth(1);
    pen.setColor(Qt::darkBlue);
    pen.setStyle(Qt::SolidLine);
    this->setPen(pen);
    this->update();
}

void EnhancedEllipseItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QStyleOptionGraphicsItem tempOption(*option);
    // qDebug()<<tempOption.state<<"====before";

    tempOption.state &=~ QStyle::State_Selected;
    tempOption.state &=~ QStyle::State_HasFocus;
    // qDebug()<<tempOption.state<<"====after";
    QGraphicsEllipseItem::paint(painter,&tempOption,widget);

    if(option->state&QStyle::State_Selected){
        QRectF boundRect = this->rect();
        QPen selectionBoxPen(Qt::black,1,Qt::DotLine);
        painter->setPen(selectionBoxPen);
        painter->setBrush(Qt::NoBrush);
        if(!m_isRotating){
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

            drawRotationHandle(painter);
        }else if(m_isRotating) {
            drawRotationHandle(painter,m_rotationCenter);
        }
    }
}

QPainterPath EnhancedEllipseItem::shape() const
{

    QPainterPath path;
    QPainterPathStroker stroker ;
    stroker.setWidth(5);
    stroker.setCapStyle(Qt::FlatCap);

    path.addRect(this->rect());
    const qreal handlePixelSize = 4;

    QRectF rotHandleRect = rotationHandleRect();
    path.addEllipse(rotHandleRect);
    qreal handleSize = 4;
    qreal halfSize= handleSize/2;
    QRectF pathBound  = this->rect();
    path.addRect(QRect(pathBound.topLeft().x()-halfSize,pathBound.topLeft().y()-halfSize,handleSize,handleSize));
    path.addRect(QRect(pathBound.topRight().x()-halfSize,pathBound.topRight().y()-halfSize,handleSize,handleSize));
    path.addRect(QRect(pathBound.bottomLeft().x()-halfSize,pathBound.bottomLeft().y()-halfSize,handleSize,handleSize));
    path.addRect(QRect(pathBound.bottomRight().x()-halfSize,pathBound.bottomRight().y()-halfSize,handleSize,handleSize));
    path.addRect(QRect((pathBound.center()+QPointF(0,pathBound.height()/2)).x()-halfSize,(pathBound.center()+QPointF(0,pathBound.height()/2)).y()-halfSize,handleSize,handleSize));
    path.addRect(QRect((pathBound.center()+QPointF(0,-pathBound.height()/2)).x()-halfSize,(pathBound.center()+QPointF(0,-pathBound.height()/2)).y()-halfSize,handleSize,handleSize));
    path.addRect(QRect((pathBound.center()+QPointF(pathBound.width()/2,0)).x()-halfSize,(pathBound.center()+QPointF(pathBound.width()/2,0)).y()-halfSize,handleSize,handleSize));
    path.addRect(QRect((pathBound.center()+QPointF(-pathBound.width()/2,0)).x()-halfSize,(pathBound.center()+QPointF(-pathBound.width()/2,0)).y()-halfSize,handleSize,handleSize));
    return path;
}

void EnhancedEllipseItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button()==Qt::LeftButton){
        this->setSelected(true);
        // 输出鼠标点击的手柄
        qDebug() << "Clicked Handle:" << handleToString(currentHandlePosition(event->pos()));
        m_activeHandle = currentHandlePosition(event->pos());
        if (m_activeHandle != None) {
            m_orginalRect = this->rect();
            if(m_activeHandle == RotationHandle){
                m_isRotating = true;
                m_rotationCenter = (this->rect().topLeft()+this->rect().bottomRight())/2;
                setCursor(Qt::CrossCursor);
                event->accept();
                qDebug() << "Rotation mode activated. Event accepted. Returning.";
                return;
            }else if (m_activeHandle == PolylineHandle){
                setCursor(Qt::CrossCursor);
                event->accept();
                qDebug() << "Polyline Dragging mode activated. Event accepted. Returning.";
                return;
            }else{
                m_startDragPos = event->pos();
                m_orginalRect = this->rect();
                setCursor(Qt::CrossCursor);
                event->accept();
                qDebug() << "Dragging mode activated. Event accepted. Returning.";
                return;
            }
        }
    }
    qDebug() << "Not on handle. Passing event to base class.";
    m_isRotating = false;
    QGraphicsEllipseItem::mousePressEvent(event);
}

void EnhancedEllipseItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(m_activeHandle!=None){
        if(m_activeHandle == RotationHandle){
            QPointF center = m_rotationCenter;
            QLineF lineToLastPos(center,event->lastPos());
            QLineF lineToCurrentPos(center,event->pos());
            qreal angelDelta = - lineToLastPos.angleTo(lineToCurrentPos);
            // 2. 设置旋转中心点
            //    这个函数指定了旋转和缩放变换的原点。
            this->setTransformOriginPoint(m_rotationCenter);
            // 3. 更新总的旋转角度
            //    获取当前的旋转角度，并累加上这次鼠标移动产生的角度变化量。
            //    QLineF::angleTo 和 QGraphicsItem::rotation 都遵循逆时针为正方向的规则，所以直接相加。
            this->setRotation(this->rotation() + angelDelta);
            event->accept();
        } else{
            QPointF currentPos = event->pos();
            QRectF newRect = m_orginalRect;
            switch (m_activeHandle){
            case TopLeft:
                newRect.setTopLeft(currentPos);
                break;
            case TopMiddle:
                newRect.setTop(currentPos.y());
                break;
            case TopRight:
                newRect.setTopRight(currentPos);
                break;
            case MiddleLeft:
                newRect.setLeft(currentPos.x());
                break;
            case MiddleRight:
                newRect.setRight(currentPos.x());
                break;
            case BottomLeft:
                newRect.setBottomLeft(currentPos);
                break;
            case BottomMiddle:
                newRect.setBottom(currentPos.y());
                break;
            case BottomRight:
                newRect.setBottomRight(currentPos);
                break;
            case None:
                break;
            case RotationHandle:
                break;
            case PolylineHandle:
                break;
            default:
                break;
            }
            if(m_orginalRect.width()==0||m_orginalRect.height()==0){
                event->accept();
                return;
            }
            qreal scaleX = newRect.width()/m_orginalRect.width();
            qreal scaleY = newRect.height()/m_orginalRect.height();
            QTransform transform ;
            transform.translate(newRect.left(),newRect.top());  // 1. 将原点移动到新矩形的左上角
            transform.scale(scaleX,scaleY);               // 2. 根据比例缩放
            transform.translate(-newRect.left(),-newRect.top());// 3. 将坐标系原点移回
            QRectF newRect2 = transform.mapRect(m_orginalRect);
            this->setRect(newRect2);
            event->accept();
        }
    }else{
        QGraphicsEllipseItem::mouseMoveEvent(event);
    }
}

void EnhancedEllipseItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(m_isRotating){
        m_isRotating=false;
        unsetCursor();
        event->accept();
    }
    m_draggedPointIndex = -1;
    m_activeHandle = None;
    QGraphicsEllipseItem::mouseReleaseEvent(event);
}

void EnhancedEllipseItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsEllipseItem::mouseDoubleClickEvent(event);

}

void EnhancedEllipseItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    HandlePosition handle = currentHandlePosition(event->pos());
    if(handle!=None){
        this->setCursor(Qt::CrossCursor);
    }else{
        this->setCursor(Qt::ArrowCursor);
    }
    QGraphicsEllipseItem::hoverMoveEvent(event);
}

void EnhancedEllipseItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    this->setCursor(Qt::ArrowCursor);
    QGraphicsEllipseItem::hoverLeaveEvent(event);
}

void EnhancedEllipseItem::drawHandle(QPainter *painter, const QPointF &pos, qreal size)
{
    qreal halfSize = size /2.0;
    painter->setPen(QPen(Qt::black,1));
    painter->setBrush(QBrush(Qt::white));
    painter->drawRect(QRect(pos.x()-halfSize,pos.y()-halfSize,size,size));
    // painter->drawPoint(pos);
}

EnhancedEllipseItem::HandlePosition EnhancedEllipseItem::currentHandlePosition(const QPointF &pos)
{
    qreal handleSize = 2;
    QRectF boundRect = this->rect();
    QPointF handles[] = {
        boundRect.topLeft(), boundRect.topRight(), boundRect.bottomLeft(), boundRect.bottomRight(),
        boundRect.center() + QPointF(0, -boundRect.height() / 2), // top
        boundRect.center() + QPointF(0, boundRect.height() / 2),  // bottom
        boundRect.center() + QPointF(-boundRect.width() / 2, 0),  // left
        boundRect.center() + QPointF(boundRect.width() / 2, 0)    // right
    };

    // 检查鼠标位置是否在某个手柄的矩形区域内
    for (int i = 0; i < 8; ++i) {
        QRectF handleRect(handles[i].x() - handleSize / 2.0, handles[i].y() - handleSize / 2.0, handleSize, handleSize);
        if (handleRect.contains(pos)) {
            // 根据索引返回对应的手柄枚举值
            switch(i){
            case 0: return TopLeft;
            case 1: return TopRight;
            case 2: return BottomLeft;
            case 3: return BottomRight;
            case 4: return TopMiddle;
            case 5: return BottomMiddle;
            case 6: return MiddleLeft;
            case 7: return MiddleRight;
            }
        }
    }
    if(rotationHandleRect().contains(pos)){
        return RotationHandle;
    }
    return None;
}

QString EnhancedEllipseItem::handleToString(HandlePosition handle)
{
    switch (handle) {
    case EnhancedEllipseItem::None: return "None";
    case EnhancedEllipseItem::TopLeft: return "TopLeft";
    case EnhancedEllipseItem::TopMiddle: return "TopMiddle";
    case EnhancedEllipseItem::TopRight: return "TopRight";
    case EnhancedEllipseItem::MiddleLeft: return "MiddleLeft";
    case EnhancedEllipseItem::MiddleRight: return "MiddleRight";
    case EnhancedEllipseItem::BottomLeft: return "BottomLeft";
    case EnhancedEllipseItem::BottomMiddle: return "BottomMiddle";
    case EnhancedEllipseItem::BottomRight: return "BottomRight";
    case EnhancedEllipseItem::RotationHandle: return "RotationHandle";
    default: return "Unknown";
    }
}

QRectF EnhancedEllipseItem::rotationHandleRect() const
{

    QRectF pathBound  = this->rect();
    QPointF direction =  pathBound .topRight() -  pathBound .topLeft();
    QPointF normalVector = QPointF(-direction.y(), direction.x());
    qreal length = std::sqrt(QPointF::dotProduct(normalVector, normalVector));
    if (length > 0) {
        normalVector /= length;
    }
    const qreal desirePixelOffset = 20.0;
    QPointF targetPoint = ( pathBound .topRight() + pathBound .topLeft())/2 - normalVector * desirePixelOffset;
    const qreal handlePixelSize = 8.0;

    return QRectF(targetPoint - QPointF(handlePixelSize / 2, handlePixelSize / 2), QSizeF(handlePixelSize, handlePixelSize));
}

void EnhancedEllipseItem::drawRotationHandle(QPainter *painter, QPointF point)
{
    QRectF handleRect = rotationHandleRect();
    const qreal lod = QStyleOptionGraphicsItem::levelOfDetailFromTransform(painter->worldTransform());
    qreal ellipseRadius = handleRect.width() / 2.0 / lod;
    QPen pen(Qt::black,1);
    pen.setCosmetic(true);
    painter->setPen(pen);
    painter->setBrush(Qt::blue);
    if(QLineF(point,QPointF(0,0)).length()!=0){
        painter->drawEllipse(m_rotationCenter,ellipseRadius,ellipseRadius);
    }else{
        painter->drawEllipse(handleRect.center(),ellipseRadius,ellipseRadius);
    }
}
