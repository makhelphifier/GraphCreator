#include "enhancedarcitem.h"
#include <QPen>
EnhancedArcItem::EnhancedArcItem() {
    this->setFlags(QGraphicsItem::ItemIsSelectable|QGraphicsItem::ItemIsMovable|QGraphicsItem::ItemIsFocusable);
    this->setAcceptHoverEvents(true);
    QPen pen;
    pen.setWidth(1);
    pen.setColor(Qt::darkBlue);
    pen.setStyle(Qt::SolidLine);
    this->setPen(pen);
    this->update();
        // 1. 创建一个 QPainterPath 对象，它就像一支画笔的轨迹
    QPainterPath path;

    // 2. 定义一个矩形区域，我们的圆弧将在这个矩形内部绘制
    QRectF rect(0, 100, 100, 100); // 一个在原点(0,0)，宽高都为100的矩形

    // 3. 在这个矩形内添加一段圆弧到路径中
    //    参数: (矩形, 开始角度, 跨越角度)  <-- 这里用的是度，不是16分之一度
    path.arcTo(rect, 30.0, 120.0); // 从30度位置开始，画一个120度的圆弧

    // 4. 将这条路径设置给我们的图形项，它就有了形状
    this->setPath(path);

    // 5. (可选，但强烈推荐)给它设置一个颜色和粗细，否则就是默认的黑色细线
    this->setPen(QPen(Qt::red, 3)); // 设置为红色画笔，线条宽度为3像素
}
