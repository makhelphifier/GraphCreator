#include "enhancedtextitem.h"

#include <QFont>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QTextCursor>


GraphTextItem::GraphTextItem(const QString &text, QGraphicsItem *parent)
{
    this->setDefaultTextColor(Qt::red);
    this->setFont(QFont("Arial",12));
    this->setPlainText(text);
    this->setFlags(QGraphicsItem::ItemIsFocusable|QGraphicsItem::ItemIsSelectable|QGraphicsItem::ItemIsMovable);
    this->setTextInteractionFlags(Qt::NoTextInteraction);
    this->update();
}
void GraphTextItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QStyleOptionGraphicsItem tempOption(*option);
    // qDebug()<<tempOption.state<<"====before";

    tempOption.state &=~ QStyle::State_Selected;
    tempOption.state &=~ QStyle::State_HasFocus;
    // qDebug()<<tempOption.state<<"====after";
    QGraphicsTextItem::paint(painter,&tempOption,widget);

    if(option->state&QStyle::State_Selected){
        QRectF boundRect = this->boundingRect();
        QPen selectionBoxPen(Qt::black,1,Qt::DotLine);
        painter->setPen(selectionBoxPen);
        painter->setBrush(Qt::NoBrush);
        painter->drawRect(boundRect);

        qreal handleSize = 6;
        drawHandle(painter,boundRect.topLeft(),handleSize);
        drawHandle(painter,boundRect.topRight(),handleSize);
        drawHandle(painter,boundRect.bottomLeft(),handleSize);
        drawHandle(painter,boundRect.bottomRight(),handleSize);
        drawHandle(painter,boundRect.center()+QPointF(0,boundRect.height()/2),handleSize);
        drawHandle(painter,boundRect.center()+QPointF(0, -boundRect.height()/2),handleSize);
        drawHandle(painter,boundRect.center()+QPointF(boundRect.width()/2,0),handleSize);
        drawHandle(painter,boundRect.center()+QPointF(-boundRect.width()/2,0),handleSize);

    }
}

void GraphTextItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button()==Qt::LeftButton){
        this->setTextInteractionFlags(Qt::TextEditorInteraction);
        this->setSelected(true);
        this->setFocus();
        this->setFlags(this->flags()&~QGraphicsItem::ItemIsMovable);
        QTextCursor cursor = this->textCursor();
        cursor.movePosition(QTextCursor::End);
        this->setTextCursor(cursor);
        event->accept();
    }else{
        QGraphicsTextItem::mouseDoubleClickEvent(event);
    }
}

void GraphTextItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

    if(event->button()==Qt::LeftButton){
        this->setSelected(true);
        // 输出鼠标点击的手柄
        qDebug() << "Clicked Handle:" << handleToString(currentHandlePosition(event->pos()));
        m_activeHandle = currentHandlePosition(event->pos());
        if (m_activeHandle != None) {
            event->accept();
        }else{
            QGraphicsTextItem::mousePressEvent(event);
        }
    }else{
        QGraphicsTextItem::mousePressEvent(event);
    }
}


void GraphTextItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    // 只有在拖动手柄时才处理
    if (m_activeHandle != None) {
        // 根据鼠标移动计算新的尺寸和位置
        QRectF newRect = this->boundingRect();
        QPointF delta = event->pos() - event->lastPos();

        // 调整矩形
        switch (m_activeHandle) {
        case TopLeft:
            newRect.setTopLeft(newRect.topLeft() + delta);
            break;
        case TopMiddle:
            newRect.setTop(newRect.top() + delta.y());
            break;
        case TopRight:
            newRect.setTopRight(newRect.topRight() + delta);
            break;
        case MiddleLeft:
            newRect.setLeft(newRect.left() + delta.x());
            break;
        case MiddleRight:
            newRect.setRight(newRect.right() + delta.x());
            break;
        case BottomLeft:
            newRect.setBottomLeft(newRect.bottomLeft() + delta);
            break;
        case BottomMiddle:
            newRect.setBottom(newRect.bottom() + delta.y());
            break;
        case BottomRight:
            newRect.setBottomRight(newRect.bottomRight() + delta);
            break;
        default:
            break;
        }

        // 确保新矩形有效（宽度和高度大于0）
        if (newRect.width() > 0 && newRect.height() > 0) {
            // 准备进行几何变化
            prepareGeometryChange();

            // 根据新矩形调整位置
            QPointF newPos = mapToParent(newRect.topLeft());
            setPos(newPos);

            // 调整宽度，QGraphicsTextItem 的高度会自动根据文本内容和宽度调整
            setTextWidth(newRect.width());
        }
        event->accept();
    } else {
        // 如果没有拖动手柄，执行父类的移动逻辑
        QGraphicsTextItem::mouseMoveEvent(event);
    }
}

void GraphTextItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    m_activeHandle = None;
    QGraphicsTextItem::mouseReleaseEvent(event);

}

void GraphTextItem::focusOutEvent(QFocusEvent *event)
{
    this->setFlags(this->flags()|QGraphicsItem::ItemIsMovable);
    this->setTextInteractionFlags(Qt::NoTextInteraction);
    QGraphicsTextItem::focusOutEvent(event);

}


void GraphTextItem::drawHandle(QPainter *painter, const QPointF &pos, qreal size)
{
    qreal halfSize = size /2.0;
    painter->setPen(QPen(Qt::black,1));
    painter->setBrush(QBrush(Qt::white));
    painter->drawRect(QRect(pos.x()-halfSize,pos.y()-halfSize,size,size));
    // painter->drawPoint(pos);
}

GraphTextItem::HandlePosition GraphTextItem::currentHandlePosition(const QPointF &pos)
{
    qreal handleSize = 4;
    QRectF boundRect = this->boundingRect();

    QPointF handles[] = {
        boundRect.topLeft(), boundRect.topRight(), boundRect.bottomLeft(), boundRect.bottomRight(),
        boundRect.center() + QPointF(0, -boundRect.height() / 2), // top
        boundRect.center() + QPointF(0, boundRect.height() / 2),  // bottom
        boundRect.center() + QPointF(-boundRect.width() / 2, 0),  // left
        boundRect.center() + QPointF(boundRect.width() / 2, 0)    // right
    };

    // 检查鼠标位置是否在某个手柄的矩形区域内
    for (int i = 0; i < 8; ++i) {
        QRectF handleRect(handles[i].x() - handleSize / 2.0, handles[i].y() - handleSize / 2.0, handleSize, handleSize);
        if (handleRect.contains(pos)) {
            // 根据索引返回对应的手柄枚举值
            switch(i){
            case 0: return TopLeft;
            case 1: return TopRight;
            case 2: return BottomLeft;
            case 3: return BottomRight;
            case 4: return TopMiddle;
            case 5: return BottomMiddle;
            case 6: return MiddleLeft;
            case 7: return MiddleRight;
            }
        }
    }
    return None;
}
QString GraphTextItem::handleToString(GraphTextItem::HandlePosition handle)
{
    switch (handle) {
    case GraphTextItem::None: return "None";
    case GraphTextItem::TopLeft: return "TopLeft";
    case GraphTextItem::TopMiddle: return "TopMiddle";
    case GraphTextItem::TopRight: return "TopRight";
    case GraphTextItem::MiddleLeft: return "MiddleLeft";
    case GraphTextItem::MiddleRight: return "MiddleRight";
    case GraphTextItem::BottomLeft: return "BottomLeft";
    case GraphTextItem::BottomMiddle: return "BottomMiddle";
    case GraphTextItem::BottomRight: return "BottomRight";
    default: return "Unknown";
    }
}
