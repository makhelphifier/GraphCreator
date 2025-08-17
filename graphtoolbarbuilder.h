#ifndef GRAPHTOOLBARBUILDER_H
#define GRAPHTOOLBARBUILDER_H

#include <QObject>
#include <QToolBar>

class GraphToolBarBuilder : public QObject
{
    Q_OBJECT
public:
    explicit GraphToolBarBuilder(QMainWindow *parent = nullptr);

    QToolBar* buildToolBar();
private:
    QMainWindow* m_parentWindow;
    QToolBar* m_toolBar;
    void createMenus();
public slots:
    void onTextTriggered();
    void onLineTriggered();
    void onPolylineTriggered();
    void onRectangleTriggered();
    void onEclipseTriggered();
    void onArcTriggered();
    void onPolygonTriggered();

signals:
    void textToolTriggered();
    void lineToolTriggered();
    void polylineToolTriggered();
    void rectangleToolTriggered();
    void eclipseToolTriggered();
    void arcToolTriggered();
    void polygonToolTriggered();

};

#endif // GRAPHTOOLBARBUILDER_H
