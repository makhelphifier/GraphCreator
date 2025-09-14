#ifndef PEN_STYLE_WIDGET_H
#define PEN_STYLE_WIDGET_H
// pen_style_widget.h
#include <QWidget>
#include <QPainter>

class PenStyleWidget : public QWidget {
    Q_OBJECT
public:
    explicit PenStyleWidget(Qt::PenStyle style, QWidget* parent = nullptr)
        : QWidget(parent), m_style(style)
    {
        setFixedSize(200, 25); // 每行大小
    }

signals:
    void activated(Qt::PenStyle style);

protected:
    void paintEvent(QPaintEvent*) override {
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);

        QPen pen(Qt::black);
        pen.setWidth(2);
        pen.setCapStyle(Qt::FlatCap);
        pen.setStyle(m_style);
        p.setPen(pen);

        int y = height() / 2;
        p.drawLine(QPoint(10, y), QPoint(width() - 10, y));
    }

    void mousePressEvent(QMouseEvent* ev) override {
        emit activated(m_style);
        QWidget::mousePressEvent(ev);
    }

private:
    Qt::PenStyle m_style;
};

#endif // PEN_STYLE_WIDGET_H
