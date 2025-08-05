#ifndef ENHANCEDLINEITEM_H
#define ENHANCEDLINEITEM_H

#include <QGraphicsLineItem>

class EnhancedLineItem : public QGraphicsLineItem
{
public:
    EnhancedLineItem();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    QPainterPath shape() const override;

private:
    void drawHandle(QPainter *painter, const QPointF &pos, qreal size);
    void drawRotationHandle(QPainter *painter);
    bool m_isRotating = false;
    QRectF rotationHandleRect() const;
    bool isPostionOnRotationHandle(const QPointF& pos) const;

};

#endif // ENHANCEDLINEITEM_H
