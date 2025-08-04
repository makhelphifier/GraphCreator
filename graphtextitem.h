#ifndef GRAPHTEXTITEM_H
#define GRAPHTEXTITEM_H

#include <QGraphicsTextItem>
class GraphTextItem : public QGraphicsTextItem
{
    Q_OBJECT
public:
    explicit GraphTextItem(const QString &text,QGraphicsItem* parent= nullptr);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;


private:
    // 辅助函数：绘制单个手柄
    void drawHandle(QPainter *painter, const QPointF &pos, qreal size);
    enum HandlePosition{
        None,TopLeft,TopMiddle,TopRight,
        MiddleLeft,MiddleRight,
        BottomLeft,BottomMiddle,BottomRight
    };
    HandlePosition m_activeHandle = None;
    HandlePosition currentHandlePosition(const QPointF &currentPos);
    QString handleToString(GraphTextItem::HandlePosition handle);
};

#endif // GRAPHTEXTITEM_H
