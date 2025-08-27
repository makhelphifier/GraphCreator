#ifndef GRAPHTOOLBARBUILDER_H
#define GRAPHTOOLBARBUILDER_H

#include <QObject>
#include <QToolBar>
#include <QColor>
#include <QEvent>
#include <QPushButton>


class GraphToolBarBuilder : public QObject
{
    Q_OBJECT
public:
    explicit GraphToolBarBuilder(QMainWindow *parent = nullptr);

    QToolBar* buildToolBar();
protected:
    bool eventFilter(QObject* watched,QEvent* event) override  ;
private:
    QMainWindow* m_parentWindow;
    QToolBar* m_toolBar;
    void createMenus();
    void createPenMenu();

    void createFillColorMenu();
    bool m_isPickingColor;
    QList<QPushButton* > m_customColorButtons;
    int m_customColorIndex;
public slots:
    void onTextTriggered();
    void onLineTriggered();
    void onPolylineTriggered();
    void onRectangleTriggered();
    void onEclipseTriggered();
    void onArcTriggered();
    void onPolygonTriggered();

private slots:
    void onPickColorTriggered();

signals:
    void textToolTriggered();
    void lineToolTriggered();
    void polylineToolTriggered();
    void rectangleToolTriggered();
    void eclipseToolTriggered();
    void arcToolTriggered();
    void polygonToolTriggered();
    void penColorChanged(const QColor& color);

};

#endif // GRAPHTOOLBARBUILDER_H
