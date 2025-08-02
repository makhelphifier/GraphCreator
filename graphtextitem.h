#ifndef GRAPHTEXTITEM_H
#define GRAPHTEXTITEM_H

#include <QGraphicsTextItem>

class GraphTextItem : public QGraphicsTextItem
{
    Q_OBJECT
public:
    explicit GraphTextItem(const QString &text,QGraphicsItem* parent= nullptr);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
private:
    // 辅助函数：绘制单个手柄
    void drawHandle(QPainter *painter, const QPointF &pos, qreal size);
};

#endif // GRAPHTEXTITEM_H
