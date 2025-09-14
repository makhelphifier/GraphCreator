#ifndef PEN_ARROW_WIDGET_H
#define PEN_ARROW_WIDGET_H

#include <QWidget>
#include <QPainter>

// 定义一个枚举来清晰地表示七种箭头样式
enum class ArrowStyle {
    NoArrow,
    RightArrow,
    LeftArrow,
    BothArrowsOut,
    RightArrowIn,
    LeftArrowIn,
    BothArrowsIn
};

class PenArrowWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PenArrowWidget(ArrowStyle style, QWidget *parent = nullptr);

signals:
    // 当控件被点击时，发射此信号并附带其代表的样式
    void activated(ArrowStyle style);

protected:
    // 重写绘图事件来绘制线条和箭头
    void paintEvent(QPaintEvent *event) override;
    // 重写鼠标点击事件以发射信号
    void mousePressEvent(QMouseEvent *event) override;
    // 重写进入和离开事件以实现悬停高亮效果
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;

private:
    void drawArrowHead(QPainter& painter, QPointF tip, double angle, bool filled);

    ArrowStyle m_style;
    bool m_isHovered = false;
};

#endif // PEN_ARROW_WIDGET_H
