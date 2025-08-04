#ifndef ENHANCEDLINEITEM_H
#define ENHANCEDLINEITEM_H

#include <QGraphicsLineItem>

class EnhancedLineItem : public QGraphicsLineItem
{
public:
    EnhancedLineItem();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    void drawHandle(QPainter *painter, const QPointF &pos, qreal size);
};

#endif // ENHANCEDLINEITEM_H
