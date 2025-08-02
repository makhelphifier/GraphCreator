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
    bool sceneEvent(QEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void focusInEvent(QFocusEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event) override;
    void dragLeaveEvent(QGraphicsSceneDragDropEvent *event) override;
    void dragMoveEvent(QGraphicsSceneDragDropEvent *event) override;
    void dropEvent(QGraphicsSceneDragDropEvent *event) override;
    void inputMethodEvent(QInputMethodEvent *event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

private:
    // 辅助函数：绘制单个手柄
    void drawHandle(QPainter *painter, const QPointF &pos, qreal size);
};

#endif // GRAPHTEXTITEM_H
