#ifndef PEN_SIZE_WIDGET_H
#define PEN_SIZE_WIDGET_H
#include <QWidget>
#include <QPen>
#include <QPainter>
class PenSizeWidget : public QWidget {
    Q_OBJECT
public:
    explicit PenSizeWidget(int size, QWidget* parent = nullptr)
        : QWidget(parent), m_size(size)
    {
        setFixedSize(140, 25); // 根据需要调整宽度
    }

signals:
    void activated(int size);

protected:
    void paintEvent(QPaintEvent*) override {
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);

        // 左侧文字
        p.setPen(Qt::black);
        QString text = QString("%1pt").arg(m_size);
        QRect textRect(5, 0, 40, height());
        p.drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, text);

        // 右侧线条
        QPen pen(Qt::black);
        pen.setWidthF(m_size);               // 支持小数宽度
        pen.setCapStyle(Qt::FlatCap);        // 避免端点外溢
        p.setPen(pen);
        int y = height() / 2;
        p.drawLine(QPoint(55, y), QPoint(width() - 10, y));
    }

    void mousePressEvent(QMouseEvent* ev) override {
        emit activated(m_size);
        QWidget::mousePressEvent(ev);
    }

private:
    int m_size;
};
#endif // PEN_SIZE_WIDGET_H
