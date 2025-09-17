#include "enhancedarcitem.h"
#include <QPen>
#include <QPainterPath>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneHoverEvent>


EnhancedArcItem::EnhancedArcItem() {

}

EnhancedArcItem::EnhancedArcItem(const QPointF &center, const QPointF &startPoint, const QPointF &endPoint, QGraphicsItem *parent)
{
    this->setFlags(QGraphicsItem::ItemIsSelectable|QGraphicsItem::ItemIsMovable|QGraphicsItem::ItemIsFocusable);
    this->setAcceptHoverEvents(true);
    QPen pen;
    pen.setWidth(1);
    pen.setColor(Qt::darkBlue);
    pen.setStyle(Qt::SolidLine);
    this->setPen(pen);
    this->update();
    setPos(center);
    m_center = QPointF(0, 0);
    m_startPoint = startPoint - center;

    qreal radius = QLineF(m_center, m_startPoint).length();
    if (radius < 1.0) return;
    m_radius = radius;  // 新增

    QRectF fullCircleRect(-radius, -radius, radius * 2, radius * 2);

    qreal startAngleDeg = QLineF(m_center, m_startPoint).angle();
    qreal endAngleDeg = QLineF(m_center, endPoint - center).angle();  // 使用传入的 endPoint 计算角度

    qreal spanAngleDeg = endAngleDeg - startAngleDeg;
    if (spanAngleDeg < 0) spanAngleDeg += 360.0;

    // 修正 m_endPoint 计算：相对旋转
    QLineF line(m_center, m_startPoint);
    line.setAngle(line.angle() - spanAngleDeg);  // 顺时针旋转
    m_endPoint = line.p2();

    // 路径构建修复
    QPainterPath piePath;
    piePath.moveTo(m_center);
    piePath.lineTo(m_startPoint);  // 先到起点
    piePath.arcTo( fullCircleRect, startAngleDeg, spanAngleDeg);  // 弧（负值为顺时针）
    piePath.lineTo(m_center);  // 回中心（显式闭合，避免 duplicate 问题）
    setPath(piePath);
    m_boundingRect = piePath.boundingRect();

    m_startAngle = static_cast<int>(startAngleDeg * 16);
    m_spanAngle = static_cast<int>(-spanAngleDeg * 16);

}

void EnhancedArcItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

    QStyleOptionGraphicsItem tempOption(*option);
    // qDebug()<<tempOption.state<<"====before";

    tempOption.state &=~ QStyle::State_Selected;
    tempOption.state &=~ QStyle::State_HasFocus;
    // qDebug()<<tempOption.state<<"====after";
    QGraphicsPathItem::paint(painter,&tempOption,widget);

    if(option->state&QStyle::State_Selected){
        QRectF boundRect = m_boundingRect;
        boundRect = boundRect;
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

            qreal halfSize = 2 /2.0;
            painter->setPen(QPen(Qt::white,0.2));
            painter->setBrush(QBrush(Qt::yellow));
            painter->drawEllipse(QRect(m_center.x()-halfSize,m_center.y()-halfSize,2,2));
            painter->drawEllipse(QRect(m_startPoint.x()-halfSize,m_startPoint.y()-halfSize,2,2));
            painter->drawEllipse(QRect(m_endPoint.x()-halfSize,m_endPoint.y()-halfSize,2,2));


            drawRotationHandle(painter);
        }else if(m_isRotating) {
            drawRotationHandle(painter,m_rotationCenter);
        }
    }
}

QPainterPath EnhancedArcItem::shape() const
{
    QPainterPath path;
    QPainterPathStroker stroker ;
    stroker.setWidth(5);
    stroker.setCapStyle(Qt::FlatCap);
    QRectF pathBound = m_boundingRect;

    path.addRect(pathBound);
    path.addPath(stroker.createStroke(this->path()));
    const qreal handlePixelSize = 8;

    path.addEllipse(m_center,handlePixelSize,handlePixelSize);
    path.addEllipse(m_startPoint,handlePixelSize,handlePixelSize);
    path.addEllipse(m_endPoint,handlePixelSize,handlePixelSize);

    QRectF rotHandleRect = rotationHandleRect();
    path.addEllipse(rotHandleRect);
    qreal handleSize = 8;
    qreal halfSize= handleSize/2;
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

QRectF EnhancedArcItem::boundingRect() const
{
    return m_boundingRect;
}

void EnhancedArcItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qreal handleSize = 8;
    qreal halfSize= handleSize/2;
    qDebug()<<event->pos();
    QRectF pathBound = m_boundingRect;

    qDebug()<<(QRect(pathBound.topLeft().x()-halfSize,pathBound.topLeft().y()-halfSize,handleSize,handleSize));
    if(event->button()==Qt::LeftButton){
        this->setSelected(true);
        // 输出鼠标点击的手柄
        qDebug() << "Clicked Handle:" << handleToString(currentHandlePosition(event->pos()));
        m_activeHandle = currentHandlePosition(event->pos());
        if (m_activeHandle != None) {
            m_orginalRect = this->path().boundingRect();
            m_orginalPath = this->path();

            if(m_activeHandle == RotationHandle){
                m_isRotating = true;
                m_rotationCenter = m_center;
                setCursor(Qt::CrossCursor);
                event->accept();
                qDebug() << "Rotation mode activated. Event accepted. Returning.";
                return;
            }else if (m_activeHandle == ArcHandle){
                for(int i =0;i<this->path().elementCount();i++){
                    const QPainterPath::Element &element = this->path().elementAt(i);
                    if(QLineF(event->pos(),element).length()<=3){
                        m_draggedPointIndex = i;
                        break;
                    }
                }
                setCursor(Qt::CrossCursor);
                event->accept();
                qDebug() << "Polyline Dragging mode activated. Event accepted. Returning.";
                return;
            } else if (m_activeHandle == CenterHandle || m_activeHandle == StartHandle || m_activeHandle == EndHandle) {
                m_startDragPos = event->scenePos();
                m_orginalRect = this->path().boundingRect();
                setCursor(Qt::CrossCursor);
                event->accept();
                qDebug() << "Dragging mode activated. Event accepted. Returning.";
                return;
            } else {
                // For resize handles
                m_original_handle_pos = getHandlePos(m_activeHandle);
                m_original_startPoint = m_startPoint;
                m_original_endPoint = m_endPoint;
                m_original_radius = m_radius;
                setCursor(Qt::CrossCursor);
                event->accept();
                return;
            }
        }
    }
    qDebug() << "Not on handle. Passing event to base class.";
    m_isRotating = false;
    QGraphicsPathItem::mousePressEvent(event);
}

void EnhancedArcItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(m_activeHandle!=None){
        if (m_activeHandle == CenterHandle) {
            setPos(event->scenePos());  // 直接设置 item pos，使中心跟随鼠标
            event->accept();
        } else if (m_activeHandle == StartHandle || m_activeHandle == EndHandle) {
            QPointF newPos = event->pos();
            QLineF line(m_center, newPos);
            line.setLength(m_radius);  // 投影到圆上

            if (m_activeHandle == StartHandle) {
                m_startPoint = line.p2();
            } else {
                m_endPoint = line.p2();
            }
            calculateGeometry();  // 重建路径
            event->accept();
        }else if(m_activeHandle == RotationHandle){
            QPointF center = m_rotationCenter;
            QLineF lineToLastPos(center,event->lastPos());
            QLineF lineToCurrentPos(center,event->pos());
            qreal angelDelta = lineToLastPos.angleTo(lineToCurrentPos);
            QTransform transform ;
            transform.translate(center.x(),center.y());  // 1. 将坐标系原点移动到旋转中心
            transform.rotate(-angelDelta);               // 2. 旋转（Qt的angle是逆时针为正，所以用负号）
            transform.translate(-center.x(),-center.y());// 3. 将坐标系原点移回
            QPainterPath path ;
            for(int i =0;i<this->path().elementCount();i++){
                const QPainterPath::Element &element = this->path().elementAt(i);
                if(i==0){
                    QPointF start = transform.map(element);
                    path.moveTo(start);
                }else{
                    QPointF temp = transform.map(element);
                    path.lineTo(temp);
                }
            }
            this->setPath(path);
            event->accept();
        } else{
            // Resize handles
            QPointF center = m_center;
            QPointF original_handle_vector = m_original_handle_pos - center;
            qreal orig_dist = QLineF(center, m_original_handle_pos).length();
            if (orig_dist == 0) {
                event->accept();
                return;
            }
            QPointF mouse_vector = event->pos() - center;
            qreal component = QPointF::dotProduct(mouse_vector, original_handle_vector) / orig_dist;
            qreal scale = component / orig_dist;
            if (scale < 0.1) scale = 0.1;

            m_startPoint = m_original_startPoint * scale;
            m_endPoint = m_original_endPoint * scale;
            m_radius = m_original_radius * scale;
            calculateGeometry();
            event->accept();
        }
    }else{
        QGraphicsPathItem::mouseMoveEvent(event);
    }
}

void EnhancedArcItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(m_isRotating){
        m_isRotating=false;
        unsetCursor();
        event->accept();
    }
    m_draggedPointIndex = -1;
    m_activeHandle = None;
    QGraphicsPathItem::mouseReleaseEvent(event);
}

void EnhancedArcItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsPathItem::mouseDoubleClickEvent(event);

}

void EnhancedArcItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    HandlePosition handle = currentHandlePosition(event->pos());
    if(handle!=None){
        this->setCursor(Qt::CrossCursor);
    }else{
        this->setCursor(Qt::ArrowCursor);

    }
    QGraphicsPathItem::hoverMoveEvent(event);

}

void EnhancedArcItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    this->setCursor(Qt::ArrowCursor);
    QGraphicsPathItem::hoverLeaveEvent(event);
}

void EnhancedArcItem::calculateGeometry()
{
    qreal startAngleDeg = QLineF(m_center, m_startPoint).angle();
    qreal endAngleDeg = QLineF(m_center, m_endPoint).angle();
    qreal spanAngleDeg = endAngleDeg - startAngleDeg;
    if (spanAngleDeg < 0) spanAngleDeg += 360.0;

    m_startAngle = static_cast<int>(startAngleDeg * 16);
    m_spanAngle = static_cast<int>(-spanAngleDeg * 16);

    QRectF fullCircleRect(-m_radius, -m_radius, m_radius * 2, m_radius * 2);

    QPainterPath piePath;
    piePath.moveTo(m_center);
    piePath.lineTo(m_startPoint);
    piePath.arcTo(fullCircleRect, startAngleDeg, -spanAngleDeg);
    piePath.lineTo(m_center);
    setPath(piePath);
    m_boundingRect = piePath.boundingRect();
}

void EnhancedArcItem::drawHandle(QPainter *painter, const QPointF &pos, qreal size)
{
    qreal halfSize = size /2.0;
    painter->setPen(QPen(Qt::black,1));
    painter->setBrush(QBrush(Qt::white));
    painter->drawRect(QRect(pos.x()-halfSize,pos.y()-halfSize,size,size));
    // painter->drawPoint(pos);
}

EnhancedArcItem::HandlePosition EnhancedArcItem::currentHandlePosition(const QPointF &pos)
{
    qreal handleSize = 8;
    QRectF boundRect = m_boundingRect;
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
    if (rotationHandleRect().contains(pos)) {
        return RotationHandle;
    }
    if (QLineF(pos, m_center).length() <= 8) {
        return CenterHandle;
    }
    if (QLineF(pos, m_startPoint).length() <= 8) {
        return StartHandle;
    }
    if (QLineF(pos, m_endPoint).length() <= 8) {
        return EndHandle;
    }
    return None;
}

QString EnhancedArcItem::handleToString(HandlePosition handle)
{
    switch (handle) {
    case EnhancedArcItem::None: return "None";
    case EnhancedArcItem::TopLeft: return "TopLeft";
    case EnhancedArcItem::TopMiddle: return "TopMiddle";
    case EnhancedArcItem::TopRight: return "TopRight";
    case EnhancedArcItem::MiddleLeft: return "MiddleLeft";
    case EnhancedArcItem::MiddleRight: return "MiddleRight";
    case EnhancedArcItem::BottomLeft: return "BottomLeft";
    case EnhancedArcItem::BottomMiddle: return "BottomMiddle";
    case EnhancedArcItem::BottomRight: return "BottomRight";
    case EnhancedArcItem::RotationHandle: return "RotationHandle";
    case EnhancedArcItem::ArcHandle: return "ArcHandle";
    case EnhancedArcItem::CenterHandle: return "CenterHandle";
    case EnhancedArcItem::StartHandle: return "StartHandle";
    case EnhancedArcItem::EndHandle: return "EndHandle";
    default: return "Unknown";
    }
}

QRectF EnhancedArcItem::rotationHandleRect() const
{
    QRectF pathBound  = this->boundingRect();
    pathBound = pathBound;
    QPointF direction =  pathBound .topRight() -  pathBound .topLeft();
    QPointF normalVector = QPointF(-direction.y(), direction.x());
    qreal length = std::sqrt(QPointF::dotProduct(normalVector, normalVector));
    if (length > 0) {
        normalVector /= length;
    }
    const qreal desirePixelOffset = 20.0;
    QPointF targetPoint = ( pathBound .topRight() + pathBound .topLeft())/2 - normalVector * desirePixelOffset;
    const qreal handlePixelSize = 12.0;

    return QRectF(targetPoint - QPointF(handlePixelSize / 2, handlePixelSize / 2), QSizeF(handlePixelSize, handlePixelSize));

}

void EnhancedArcItem::drawRotationHandle(QPainter *painter, QPointF point)
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

QPointF EnhancedArcItem::getHandlePos(HandlePosition handle) const
{
    QRectF boundRect = m_boundingRect;
    switch (handle) {
    case TopLeft: return boundRect.topLeft();
    case TopRight: return boundRect.topRight();
    case BottomLeft: return boundRect.bottomLeft();
    case BottomRight: return boundRect.bottomRight();
    case TopMiddle: return boundRect.center() + QPointF(0, -boundRect.height() / 2);
    case BottomMiddle: return boundRect.center() + QPointF(0, boundRect.height() / 2);
    case MiddleLeft: return boundRect.center() + QPointF(-boundRect.width() / 2, 0);
    case MiddleRight: return boundRect.center() + QPointF(boundRect.width() / 2, 0);
    default: return QPointF();
    }
}
QPointF EnhancedArcItem::startPoint() const
{
    return m_startPoint;
}

QPointF EnhancedArcItem::endPoint() const
{
    return m_endPoint;
}
