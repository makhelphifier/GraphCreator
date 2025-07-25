#ifndef NEWGRAPHWIDGET_H
#define NEWGRAPHWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QTreeWidget>
#include <QListWidget>
#include <QLineEdit>



class NewGraphWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NewGraphWidget(QWidget *parent = nullptr);


private:
    QPushButton* newButton;
    QPushButton* cancelButton;
    QLabel* textLabel;
    QTreeWidget* graphTree;
    QListWidget* graphList;
    QLineEdit* lineEdit;

    void initTreeWidget();


signals:


};

#endif // NEWGRAPHWIDGET_H
