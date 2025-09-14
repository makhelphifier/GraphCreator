#include "penarrowwidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <cmath> // for std::atan2, std::cos, std::sin

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

PenArrowWidget::PenArrowWidget(ArrowStyle style, QWidget *parent)
    : QWidget(parent), m_style(style)
{
    setFixedSize(120, 25);
    setMouseTracking(true); // 启用鼠标跟踪以响应悬停
}

void PenArrowWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 如果鼠标悬停，绘制背景
    if (m_isHovered) {
        painter.fillRect(rect(), palette().highlight());
    }

    // 设置画笔
    QPen pen(Qt::black, 2);
    pen.setCapStyle(Qt::FlatCap);
    painter.setPen(pen);
    // 设置画刷用于填充箭头
    painter.setBrush(Qt::black);

    // 定义线条的起点和终点
    QPointF start(15, height() / 2);
    QPointF end(width() - 15, height() / 2);

    // 绘制主线
    painter.drawLine(start, end);

    // 根据样式绘制箭头
    double angle = std::atan2(end.y() - start.y(), end.x() - start.x());

    switch (m_style) {
    case ArrowStyle::NoArrow:
        break;
    case ArrowStyle::RightArrow:
        drawArrowHead(painter, end, angle, true);
        break;
    case ArrowStyle::LeftArrow:
        drawArrowHead(painter, start, angle + M_PI, true);
        break;
    case ArrowStyle::BothArrowsOut:
        drawArrowHead(painter, end, angle, true);
        drawArrowHead(painter, start, angle + M_PI, true);
        break;
    case ArrowStyle::RightArrowIn:
        drawArrowHead(painter, end, angle + M_PI, true);
        break;
    case ArrowStyle::LeftArrowIn:
        drawArrowHead(painter, start, angle, true);
        break;
    case ArrowStyle::BothArrowsIn:
        drawArrowHead(painter, end, angle + M_PI, true);
        drawArrowHead(painter, start, angle, true);
        break;
    }
}

// 绘制箭头的辅助函数
void PenArrowWidget::drawArrowHead(QPainter& painter, QPointF tip, double angle, bool filled)
{
    double arrowSize = 8.0;
    // 计算箭头的两个翼点
    QPointF p1 = tip - QPointF(arrowSize * std::cos(angle - M_PI / 6), arrowSize * std::sin(angle - M_PI / 6));
    QPointF p2 = tip - QPointF(arrowSize * std::cos(angle + M_PI / 6), arrowSize * std::sin(angle + M_PI / 6));

    QPolygonF arrowHead;
    arrowHead << tip << p1 << p2;

    if (filled) {
        painter.drawPolygon(arrowHead);
    } else {
        painter.drawLine(tip, p1);
        painter.drawLine(tip, p2);
    }
}


void PenArrowWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        emit activated(m_style);
    }
    QWidget::mousePressEvent(event);
}

void PenArrowWidget::enterEvent(QEnterEvent *event)
{
    m_isHovered = true;
    update(); // 重新绘制以显示高亮
    QWidget::enterEvent(event);
}

void PenArrowWidget::leaveEvent(QEvent *event)
{
    m_isHovered = false;
    update(); // 重新绘制以移除高亮
    QWidget::leaveEvent(event);
}
