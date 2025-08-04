#include "graphtextitem.h"


GraphTextItem::GraphTextItem(const QString &text, QGraphicsItem *parent)
{
    this->setPlainText(text);
    this->setFlags(QGraphicsItem::ItemIsFocusable|QGraphicsItem::ItemIsMovable|QGraphicsItem::ItemIsFocusable);

}
