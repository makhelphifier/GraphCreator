#include "enhancedpolylineitem.h"
#include <QPen>
#include <QStyle>
#include <QStyleOptionGraphicsItem>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QRectF>
#include <QLineF>
#include <QTransform>


EnhancedPolylineItem::EnhancedPolylineItem() {

    this->setFlags(QGraphicsItem::ItemIsSelectable|QGraphicsItem::ItemIsMovable|QGraphicsItem::ItemIsFocusable);
    this->setAcceptHoverEvents(true);
    QPen pen;
    pen.setWidth(1);
    pen.setColor(Qt::darkBlue);
    pen.setStyle(Qt::SolidLine);
    this->setPen(pen);
    this->update();
}

void EnhancedPolylineItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
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
                m_rotationCenter = (this->path().boundingRect().topLeft()+this->path().boundingRect().bottomRight())/2;
                setCursor(Qt::CrossCursor);
                event->accept();
                qDebug() << "Rotation mode activated. Event accepted. Returning.";
                return;
            }else if (m_activeHandle == PolylineHandle){
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
            }else{
                m_startDragPos = event->pos();
                m_orginalRect = this->path().boundingRect();
                setCursor(Qt::CrossCursor);
                event->accept();
                qDebug() << "Dragging mode activated. Event accepted. Returning.";
                return;
            }
        }
    }
    qDebug() << "Not on handle. Passing event to base class.";
    m_isRotating = false;
    QGraphicsPathItem::mousePressEvent(event);
}

void EnhancedPolylineItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(m_activeHandle!=None){
        if(m_activeHandle == PolylineHandle ){
            QPainterPath path ;

            for(int i =0;i<this->path().elementCount();i++){
                const QPainterPath::Element &element = this->path().elementAt(i);
                QPointF point ;
                if(i == m_draggedPointIndex){
                    point = event->pos();
                }else{
                    point = QPointF(element.x,element.y);
                }

                if(i==0){
                    path.moveTo(point);
                }else {
                    path.lineTo(point);
                }
            }
            this->setPath(path);
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

            // QPainterPath oldPath = QGraphicsPathItem::path();
            QPainterPath path = transform.map(m_orginalPath);
            this->setPath(path);
            event->accept();
        }
    }else{
        QGraphicsPathItem::mouseMoveEvent(event);
    }

}

void EnhancedPolylineItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
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

void EnhancedPolylineItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsPathItem::mouseDoubleClickEvent(event);

}

void EnhancedPolylineItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    HandlePosition handle = currentHandlePosition(event->pos());
    if(handle!=None){
        this->setCursor(Qt::CrossCursor);
    }else{
        this->setCursor(Qt::ArrowCursor);
    }
    QGraphicsPathItem::hoverMoveEvent(event);
}

void EnhancedPolylineItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    this->setCursor(Qt::ArrowCursor);
    QGraphicsPathItem::hoverLeaveEvent(event);
}

QPainterPath EnhancedPolylineItem::shape() const
{
    QPainterPath path;
    QPainterPathStroker stroker ;
    stroker.setWidth(5);
    stroker.setCapStyle(Qt::FlatCap);

    path.addPath(stroker.createStroke(this->path()));
    const qreal handlePixelSize = 4;
    for(int i =0;i< this->path().elementCount();i++){
        const QPainterPath::Element &element = this->path().elementAt(i);
        QPointF point =QPointF( element.x, element.y);
        path.addEllipse(point,handlePixelSize,handlePixelSize);
    }
    QRectF rotHandleRect = rotationHandleRect();
    path.addEllipse(rotHandleRect);
    qreal handleSize = 4;
    qreal halfSize= handleSize/2;
    QRectF pathBound  = this->path().boundingRect();
    pathBound = pathBound.adjusted(-5,-5,5,5);
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

QRectF EnhancedPolylineItem::rotationHandleRect() const
{

    QRectF pathBound  = this->path().boundingRect();
    pathBound = pathBound.adjusted(-5,-5,5,5);
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

void EnhancedPolylineItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QStyleOptionGraphicsItem tempOption(*option);
    // qDebug()<<tempOption.state<<"====before";

    tempOption.state &=~ QStyle::State_Selected;
    tempOption.state &=~ QStyle::State_HasFocus;
    // qDebug()<<tempOption.state<<"====after";
    QGraphicsPathItem::paint(painter,&tempOption,widget);

    if(option->state&QStyle::State_Selected){
        QRectF boundRect = this->path().boundingRect();
        boundRect = boundRect.adjusted(-5,-5,5,5);
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
            for(int i =0;i<this->path().elementCount();i++){
                const QPainterPath::Element &element = this->path().elementAt(i);
                qreal halfSize = 2 /2.0;
                painter->setPen(QPen(Qt::white,0.2));
                painter->setBrush(QBrush(Qt::black));
                painter->drawEllipse(QRect(element.x-halfSize,element.y-halfSize,2,2));
            }
            drawRotationHandle(painter);
        }else if(m_isRotating) {
            drawRotationHandle(painter,m_rotationCenter);
        }
    }
}

void EnhancedPolylineItem::drawHandle(QPainter *painter,const  QPointF pos, qreal size)
{
    qreal halfSize = size /2.0;
    painter->setPen(QPen(Qt::black,1));
    painter->setBrush(QBrush(Qt::white));
    painter->drawRect(QRect(pos.x()-halfSize,pos.y()-halfSize,size,size));
    // painter->drawPoint(pos);
}

void EnhancedPolylineItem::drawRotationHandle(QPainter *painter,QPointF point)
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

EnhancedPolylineItem::HandlePosition EnhancedPolylineItem::currentHandlePosition(const QPointF &pos)
{
    qreal handleSize = 2;
    QRectF boundRect = this->path().boundingRect();
    boundRect = boundRect.adjusted(-5,-5,5,5);
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
    for(int i =0;i<this->path().elementCount();i++){
        const QPainterPath::Element &element = this->path().elementAt(i);
        if(QLineF(pos,element).length()<=3){
            return PolylineHandle;
        }
    }
    return None;
}

QString EnhancedPolylineItem::handleToString(EnhancedPolylineItem::HandlePosition handle)
{
    switch (handle) {
    case EnhancedPolylineItem::None: return "None";
    case EnhancedPolylineItem::TopLeft: return "TopLeft";
    case EnhancedPolylineItem::TopMiddle: return "TopMiddle";
    case EnhancedPolylineItem::TopRight: return "TopRight";
    case EnhancedPolylineItem::MiddleLeft: return "MiddleLeft";
    case EnhancedPolylineItem::MiddleRight: return "MiddleRight";
    case EnhancedPolylineItem::BottomLeft: return "BottomLeft";
    case EnhancedPolylineItem::BottomMiddle: return "BottomMiddle";
    case EnhancedPolylineItem::BottomRight: return "BottomRight";
    case EnhancedPolylineItem::RotationHandle: return "RotationHandle";
    case EnhancedPolylineItem::PolylineHandle: return "PolylineHandle";
    default: return "Unknown";
    }
}
