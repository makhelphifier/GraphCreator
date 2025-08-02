#ifndef GRAPHTEXTITEM_H
#define GRAPHTEXTITEM_H

#include <QGraphicsTextItem>

class GraphTextItem : public QGraphicsTextItem
{
    Q_OBJECT
public:
    explicit GraphTextItem();
    explicit GraphTextItem(QGraphicsItem* parent= nullptr);
    explicit GraphTextItem(const QString &text,QGraphicsItem* parent= nullptr);

};

#endif // GRAPHTEXTITEM_H
