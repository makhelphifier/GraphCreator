#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QGraphicsScene* m_scene ;
protected:
    void wheelEvent(QWheelEvent *event) override;

private:
    QGraphicsView* m_view ;
};
#endif // MAINWINDOW_H
