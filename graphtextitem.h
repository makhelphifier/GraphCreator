#ifndef GRAPHTEXTITEM_H
#define GRAPHTEXTITEM_H

#include <QGraphicsTextItem>

class GraphTextItem : public QGraphicsTextItem
{
    Q_OBJECT
public:
    explicit GraphTextItem(const QString &text,QGraphicsItem* parent= nullptr);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
    // 辅助函数：绘制单个手柄
    void drawHandle(QPainter *painter, const QPointF &pos, qreal size);
};

#endif // GRAPHTEXTITEM_H
