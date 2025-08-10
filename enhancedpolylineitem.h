#ifndef ENHANCEDPOLYLINEITEM_H
#define ENHANCEDPOLYLINEITEM_H

#include <QGraphicsPathItem>

class EnhancedPolylineItem : public QGraphicsPathItem
{
public:
    EnhancedPolylineItem();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

protected:
    QPainterPath shape() const override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
private:
    void drawHandle(QPainter *, QPointF, qreal);
    enum HandlePosition{
        None,
        TopLeft,TopMiddle,TopRight,
        MiddleLeft,MiddleRight,
        BottomLeft,BottomMiddle,BottomRight,
        RotationHandle,
        PolylineHandle
    };
    EnhancedPolylineItem::HandlePosition currentHandlePosition(const QPointF &pos);
    HandlePosition m_activeHandle;

    QString handleToString(EnhancedPolylineItem::HandlePosition handle);
    QRectF rotationHandleRect() const;
    void drawRotationHandle(QPainter *painter,QPointF point = QPointF(0,0));
    bool m_isRotating = false;
    QPointF m_rotationCenter;
    int m_draggedPointIndex = -1;
    QPointF m_startDragPos;
    QRectF m_orginalRect;
};

#endif // ENHANCEDPOLYLINEITEM_H
