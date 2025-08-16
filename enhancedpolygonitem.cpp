#include "enhancedpolygonitem.h"
#include <QPen>
#include <QStyle>
#include <QStyleOptionGraphicsItem>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QRectF>
#include <QLineF>
#include <QTransform>

EnhancedPolygonItem::EnhancedPolygonItem() {
    this->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsFocusable);
    this->setAcceptHoverEvents(true);
    QPen pen;
    pen.setWidth(1);
    pen.setColor(Qt::darkBlue);
    pen.setStyle(Qt::SolidLine);
    this->setPen(pen);
    this->update();
}

void EnhancedPolygonItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        this->setSelected(true);
        // 输出鼠标点击的手柄
        qDebug() << "Clicked Handle:" << handleToString(currentHandlePosition(event->pos()));
        m_activeHandle = currentHandlePosition(event->pos());
        if (m_activeHandle != None) {
            m_originalRect = this->path().boundingRect();
            m_originalPath = this->path();

            if (m_activeHandle == RotationHandle) {
                m_isRotating = true;
                m_rotationCenter = (this->path().boundingRect().topLeft() + this->path().boundingRect().bottomRight()) / 2;
                setCursor(Qt::CrossCursor);
                event->accept();
                qDebug() << "Rotation mode activated. Event accepted. Returning.";
                return;
            } else if (m_activeHandle == PolygonHandle) {
                for (int i = 0; i < this->path().elementCount(); i++) {
                    const QPainterPath::Element &element = this->path().elementAt(i);
                    if (QLineF(event->pos(), QPointF(element.x, element.y)).length() <= 3) {
                        m_draggedPointIndex = i;
                        break;
                    }
                }
                setCursor(Qt::CrossCursor);
                event->accept();
                qDebug() << "Polygon Dragging mode activated. Event accepted. Returning.";
                return;
            } else {
                m_startDragPos = event->pos();
                m_originalRect = this->path().boundingRect();
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

void EnhancedPolygonItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if (m_activeHandle != None) {
        if (m_activeHandle == PolygonHandle) {
            // 获取当前路径的点
            const QPainterPath currentPath = this->path();
            const int elementCount = currentPath.elementCount();
            if (elementCount < 2) {
                QGraphicsPathItem::mouseMoveEvent(event);
                return;
            }

            // 收集所有点到 vector
            QVector<QPointF> points;
            for (int i = 0; i < elementCount; ++i) {
                const QPainterPath::Element &element = currentPath.elementAt(i);
                points.append(QPointF(element.x, element.y));
            }

            // 检查是否是 duplicate 闭合（第一个和最后一个点相同）
            bool isDuplicateClosed = (points.first() == points.last());

            // 更新被拖动的点
            points[m_draggedPointIndex] = event->pos();

            // 如果是 duplicate 闭合，同步起点和终点
            if (isDuplicateClosed) {
                if (m_draggedPointIndex == 0) {
                    points.last() = points.first();  // 同步终点到新起点
                } else if (m_draggedPointIndex == elementCount - 1) {
                    points.first() = points.last();  // 同步起点到新终点
                }
            }

            // 重新构建路径
            QPainterPath newPath;
            newPath.moveTo(points[0]);
            for (int i = 1; i < points.size(); ++i) {
                newPath.lineTo(points[i]);
            }
            newPath.closeSubpath();  // 始终闭合，确保兼容两种情况

            this->setPath(newPath);
            event->accept();
        }else if (m_activeHandle == RotationHandle) {
            QPointF center = m_rotationCenter;
            QLineF lineToLastPos(center, event->lastPos());
            QLineF lineToCurrentPos(center, event->pos());
            qreal angleDelta = lineToLastPos.angleTo(lineToCurrentPos);
            QTransform transform;
            transform.translate(center.x(), center.y());  // 1. Move origin to rotation center
            transform.rotate(-angleDelta);                // 2. Rotate (negative for Qt's clockwise)
            transform.translate(-center.x(), -center.y()); // 3. Move origin back
            QPainterPath path;
            for (int i = 0; i < this->path().elementCount(); i++) {
                const QPainterPath::Element &element = this->path().elementAt(i);
                QPointF temp = transform.map(QPointF(element.x, element.y));
                if (i == 0) {
                    path.moveTo(temp);
                } else {
                    path.lineTo(temp);
                }
            }
            path.closeSubpath(); // Ensure the polygon is closed
            this->setPath(path);
            event->accept();
        } else {
            QPointF currentPos = event->pos();
            QRectF newRect = m_originalRect;
            switch (m_activeHandle) {
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
            default:
                break;
            }
            if (m_originalRect.width() == 0 || m_originalRect.height() == 0) {
                event->accept();
                return;
            }
            qreal scaleX = newRect.width() / m_originalRect.width();
            qreal scaleY = newRect.height() / m_originalRect.height();
            QTransform transform;
            transform.translate(newRect.left(), newRect.top());  // 1. Move to new top-left
            transform.scale(scaleX, scaleY);                     // 2. Scale
            transform.translate(-newRect.left(), -newRect.top());// 3. Move back
            QPainterPath path = transform.map(m_originalPath);
            this->setPath(path);
            event->accept();
        }
    } else {
        QGraphicsPathItem::mouseMoveEvent(event);
    }
}

void EnhancedPolygonItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    if (m_isRotating) {
        m_isRotating = false;
        unsetCursor();
        event->accept();
    }
    m_draggedPointIndex = -1;
    m_activeHandle = None;
    QGraphicsPathItem::mouseReleaseEvent(event);
}

void EnhancedPolygonItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsPathItem::mouseDoubleClickEvent(event);
}

void EnhancedPolygonItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event) {
    HandlePosition handle = currentHandlePosition(event->pos());
    if (handle != None) {
        this->setCursor(Qt::CrossCursor);
    } else {
        this->setCursor(Qt::ArrowCursor);
    }
    QGraphicsPathItem::hoverMoveEvent(event);
}

void EnhancedPolygonItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    this->setCursor(Qt::ArrowCursor);
    QGraphicsPathItem::hoverLeaveEvent(event);
}

QPainterPath EnhancedPolygonItem::shape() const {
    QPainterPath path;
    QPainterPathStroker stroker;
    stroker.setWidth(5);
    stroker.setCapStyle(Qt::FlatCap);
    path.addPath(stroker.createStroke(this->path()));
    const qreal handlePixelSize = 4;
    for (int i = 0; i < this->path().elementCount(); i++) {
        const QPainterPath::Element &element = this->path().elementAt(i);
        QPointF point = QPointF(element.x, element.y);
        path.addEllipse(point, handlePixelSize, handlePixelSize);
    }
    QRectF rotHandleRect = rotationHandleRect();
    path.addEllipse(rotHandleRect);
    qreal handleSize = 4;
    qreal halfSize = handleSize / 2;
    QRectF pathBound = this->path().boundingRect();
    pathBound = pathBound.adjusted(-5, -5, 5, 5);
    path.addRect(QRectF(pathBound.topLeft().x() - halfSize, pathBound.topLeft().y() - halfSize, handleSize, handleSize));
    path.addRect(QRectF(pathBound.topRight().x() - halfSize, pathBound.topRight().y() - halfSize, handleSize, handleSize));
    path.addRect(QRectF(pathBound.bottomLeft().x() - halfSize, pathBound.bottomLeft().y() - halfSize, handleSize, handleSize));
    path.addRect(QRectF(pathBound.bottomRight().x() - halfSize, pathBound.bottomRight().y() - halfSize, handleSize, handleSize));
    path.addRect(QRectF((pathBound.center() + QPointF(0, pathBound.height() / 2)).x() - halfSize, (pathBound.center() + QPointF(0, pathBound.height() / 2)).y() - halfSize, handleSize, handleSize));
    path.addRect(QRectF((pathBound.center() + QPointF(0, -pathBound.height() / 2)).x() - halfSize, (pathBound.center() + QPointF(0, -pathBound.height() / 2)).y() - halfSize, handleSize, handleSize));
    path.addRect(QRectF((pathBound.center() + QPointF(pathBound.width() / 2, 0)).x() - halfSize, (pathBound.center() + QPointF(pathBound.width() / 2, 0)).y() - halfSize, handleSize, handleSize));
    path.addRect(QRectF((pathBound.center() + QPointF(-pathBound.width() / 2, 0)).x() - halfSize, (pathBound.center() + QPointF(-pathBound.width() / 2, 0)).y() - halfSize, handleSize, handleSize));
    return path;
}

QRectF EnhancedPolygonItem::rotationHandleRect() const {
    QRectF pathBound = this->path().boundingRect();
    pathBound = pathBound.adjusted(-5, -5, 5, 5);
    QPointF direction = pathBound.topRight() - pathBound.topLeft();
    QPointF normalVector = QPointF(-direction.y(), direction.x());
    qreal length = std::sqrt(QPointF::dotProduct(normalVector, normalVector));
    if (length > 0) {
        normalVector /= length;
    }
    const qreal desirePixelOffset = 20.0;
    QPointF targetPoint = (pathBound.topRight() + pathBound.topLeft()) / 2 - normalVector * desirePixelOffset;
    const qreal handlePixelSize = 8.0;
    return QRectF(targetPoint - QPointF(handlePixelSize / 2, handlePixelSize / 2), QSizeF(handlePixelSize, handlePixelSize));
}

void EnhancedPolygonItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    QStyleOptionGraphicsItem tempOption(*option);
    tempOption.state &= ~QStyle::State_Selected;
    tempOption.state &= ~QStyle::State_HasFocus;
    QGraphicsPathItem::paint(painter, &tempOption, widget);

    if (option->state & QStyle::State_Selected) {
        QRectF boundRect = this->path().boundingRect();
        boundRect = boundRect.adjusted(-5, -5, 5, 5);
        QPen selectionBoxPen(Qt::black, 1, Qt::DotLine);
        painter->setPen(selectionBoxPen);
        painter->setBrush(Qt::NoBrush);
        if (!m_isRotating && m_draggedPointIndex == -1) {
            painter->drawRect(boundRect);
            qreal handleSize = 4;
            drawHandle(painter, boundRect.topLeft(), handleSize);
            drawHandle(painter, boundRect.topRight(), handleSize);
            drawHandle(painter, boundRect.bottomLeft(), handleSize);
            drawHandle(painter, boundRect.bottomRight(), handleSize);
            drawHandle(painter, boundRect.center() + QPointF(0, boundRect.height() / 2), handleSize);
            drawHandle(painter, boundRect.center() + QPointF(0, -boundRect.height() / 2), handleSize);
            drawHandle(painter, boundRect.center() + QPointF(boundRect.width() / 2, 0), handleSize);
            drawHandle(painter, boundRect.center() + QPointF(-boundRect.width() / 2, 0), handleSize);
            for (int i = 0; i < this->path().elementCount(); i++) {
                const QPainterPath::Element &element = this->path().elementAt(i);
                qreal halfSize = 2 / 2.0;
                painter->setPen(QPen(Qt::white, 0.2));
                painter->setBrush(QBrush(Qt::black));
                painter->drawEllipse(QRectF(element.x - halfSize, element.y - halfSize, 2, 2));
            }
            drawRotationHandle(painter);
        } else if (m_isRotating) {
            drawRotationHandle(painter, m_rotationCenter);
        }
    }
}

void EnhancedPolygonItem::drawHandle(QPainter *painter, const QPointF pos, qreal size) {
    qreal halfSize = size / 2.0;
    painter->setPen(QPen(Qt::black, 1));
    painter->setBrush(QBrush(Qt::white));
    painter->drawRect(QRectF(pos.x() - halfSize, pos.y() - halfSize, size, size));
}

void EnhancedPolygonItem::drawRotationHandle(QPainter *painter, QPointF point) {
    QRectF handleRect = rotationHandleRect();
    const qreal lod = QStyleOptionGraphicsItem::levelOfDetailFromTransform(painter->worldTransform());
    qreal ellipseRadius = handleRect.width() / 2.0 / lod;
    QPen pen(Qt::black, 1);
    pen.setCosmetic(true);
    painter->setPen(pen);
    painter->setBrush(Qt::blue);
    if (QLineF(point, QPointF(0, 0)).length() != 0) {
        painter->drawEllipse(m_rotationCenter, ellipseRadius, ellipseRadius);
    } else {
        painter->drawEllipse(handleRect.center(), ellipseRadius, ellipseRadius);
    }
}

EnhancedPolygonItem::HandlePosition EnhancedPolygonItem::currentHandlePosition(const QPointF &pos) {
    qreal handleSize = 2;
    QRectF boundRect = this->path().boundingRect();
    boundRect = boundRect.adjusted(-5, -5, 5, 5);
    QPointF handles[] = {
        boundRect.topLeft(), boundRect.topRight(), boundRect.bottomLeft(), boundRect.bottomRight(),
        boundRect.center() + QPointF(0, -boundRect.height() / 2), // top
        boundRect.center() + QPointF(0, boundRect.height() / 2),  // bottom
        boundRect.center() + QPointF(-boundRect.width() / 2, 0),  // left
        boundRect.center() + QPointF(boundRect.width() / 2, 0)    // right
    };

    // Check if mouse is within a handle rect
    for (int i = 0; i < 8; ++i) {
        QRectF handleRect(handles[i].x() - handleSize / 2.0, handles[i].y() - handleSize / 2.0, handleSize, handleSize);
        if (handleRect.contains(pos)) {
            switch (i) {
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
    for (int i = 0; i < this->path().elementCount(); i++) {
        const QPainterPath::Element &element = this->path().elementAt(i);
        if (QLineF(pos, QPointF(element.x, element.y)).length() <= 3) {
            return PolygonHandle;
        }
    }
    return None;
}

QString EnhancedPolygonItem::handleToString(EnhancedPolygonItem::HandlePosition handle) {
    switch (handle) {
    case EnhancedPolygonItem::None: return "None";
    case EnhancedPolygonItem::TopLeft: return "TopLeft";
    case EnhancedPolygonItem::TopMiddle: return "TopMiddle";
    case EnhancedPolygonItem::TopRight: return "TopRight";
    case EnhancedPolygonItem::MiddleLeft: return "MiddleLeft";
    case EnhancedPolygonItem::MiddleRight: return "MiddleRight";
    case EnhancedPolygonItem::BottomLeft: return "BottomLeft";
    case EnhancedPolygonItem::BottomMiddle: return "BottomMiddle";
    case EnhancedPolygonItem::BottomRight: return "BottomRight";
    case EnhancedPolygonItem::RotationHandle: return "RotationHandle";
    case EnhancedPolygonItem::PolygonHandle: return "PolygonHandle";
    default: return "Unknown";
    }
}
