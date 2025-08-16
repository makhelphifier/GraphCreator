#ifndef ENHANCEDARCITEM_H
#define ENHANCEDARCITEM_H

#include <QGraphicsPathItem>

class EnhancedArcItem : public QGraphicsPathItem
{
public:
    EnhancedArcItem();
    EnhancedArcItem(const QPointF& center, const QPointF& startPoint, const QPointF& endPoint, QGraphicsItem* parent = nullptr);
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
    QPainterPath shape() const override;
    QRectF boundingRect() const override;
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
private:
    // 一个辅助函数，用于根据三点计算绘图所需的参数
    void calculateGeometry();

    // 存储绘图参数
    QRectF m_boundingRect; // 外接矩形
    int m_startAngle;      // 起始角度 (1/16度)
    int m_spanAngle;       // 跨越角度 (1/16度)
    void drawHandle(QPainter *painter,const QPointF& pos, qreal size);
    enum HandlePosition{
        None,
        TopLeft,TopMiddle,TopRight,
        MiddleLeft,MiddleRight,
        BottomLeft,BottomMiddle,BottomRight,
        RotationHandle,
        ArcHandle,
        CenterHandle,  // 新增：中心
        StartHandle,   // 新增：起点
        EndHandle      // 新增：终点
    };
    EnhancedArcItem::HandlePosition currentHandlePosition(const QPointF &pos);
    HandlePosition m_activeHandle;

    QString handleToString(EnhancedArcItem::HandlePosition handle);
    QRectF rotationHandleRect() const;
    void drawRotationHandle(QPainter *painter,QPointF point = QPointF(0,0));
    bool m_isRotating = false;
    QPointF m_rotationCenter;
    int m_draggedPointIndex = -1;
    QPointF m_startDragPos;
    QRectF m_orginalRect;
    QPointF m_center;
    QPointF m_startPoint;
    QPointF m_endPoint;
    QPointF m_original_startPoint;
    QPointF m_original_endPoint;
    QPointF m_original_handle_pos;

    qreal m_radius;
    qreal m_original_radius;
    QPainterPath m_orginalPath;
    QPointF getHandlePos(HandlePosition handle) const;
};

#endif // ENHANCEDARCITEM_H
