#ifndef ENHANCEDELLIPSEITEM_H
#define ENHANCEDELLIPSEITEM_H

#include <QGraphicsEllipseItem>

class EnhancedEllipseItem : public QGraphicsEllipseItem
{
public:
    EnhancedEllipseItem();
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
    void drawHandle(QPainter *painter,const QPointF& pos, qreal size);
    enum HandlePosition{
        None,
        TopLeft,TopMiddle,TopRight,
        MiddleLeft,MiddleRight,
        BottomLeft,BottomMiddle,BottomRight,
        RotationHandle,
        PolylineHandle
    };
    EnhancedEllipseItem::HandlePosition currentHandlePosition(const QPointF &pos);
    HandlePosition m_activeHandle;

    QString handleToString(EnhancedEllipseItem::HandlePosition handle);
    QRectF rotationHandleRect() const;
    void drawRotationHandle(QPainter *painter,QPointF point = QPointF(0,0));
    bool m_isRotating = false;
    QPointF m_rotationCenter;
    int m_draggedPointIndex = -1;
    QPointF m_startDragPos;
    QRectF m_orginalRect;
    QPainterPath m_orginalPath;
};

#endif // ENHANCEDELLIPSEITEM_H
