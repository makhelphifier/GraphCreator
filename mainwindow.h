#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>

class EnhancedTextItem;
class EnhancedLineItem;
class EnhancedPolylineItem;
class EnhancedRectangleItem;
class EnhancedEllipseItem;
class EnhancedArcItem;
class EnhancedPolygonItem;


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    enum DrawingMode {
        NoMode,
        DrawText,
        DrawLine,
        DrawPolyline,
        DrawRectangle,
        DrawEllipse,
        DrawArc,
        DrawPolygon
    };
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QGraphicsScene* m_scene ;
    QGraphicsScene* scene() const;
protected:
    bool eventFilter(QObject* watched,QEvent* event) override;
    void wheelEvent(QWheelEvent *event) override;

public slots:
    void enterTextDrawingMode();
    void enterLineDrawingMode();
    void enterPolylineDrawingMode();
    void enterRectangleDrawingMode();
    void enterEllipseDrawingMode();
    void enterArcDrawingMode();
    void enterPolygonDrawingMode();
    void setCurrentFile(const QString &filePath);

private:
    QGraphicsView* m_view ;
    DrawingMode m_currentMode = NoMode;
    QPointF m_startPoint;
    QPointF m_EndPoint;
    QPainterPath* m_polylinePath = nullptr;
    QPainterPath* m_polygonPath = nullptr;
    QVector<QPointF>* m_polyPointList = new QVector<QPointF>;
    EnhancedTextItem* m_currentItem = nullptr;
    EnhancedLineItem* m_currentLineItem = nullptr;
    EnhancedPolylineItem* m_currentPolylineItem = nullptr;
    EnhancedRectangleItem* m_currentRectangleItem = nullptr;
    EnhancedEllipseItem* m_currentEllipseItem = nullptr;
    EnhancedArcItem* m_currentArcItem = nullptr;
    EnhancedPolygonItem* m_currentPolygonItem = nullptr;
    QString m_currentFilePath;
    void removeLastPointFromPath(QPainterPath &path);
};
#endif // MAINWINDOW_H
