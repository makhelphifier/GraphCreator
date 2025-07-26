#ifndef NEWGRAPHWIDGET_H
#define NEWGRAPHWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QTreeWidget>
#include <QListWidget>
#include <QLineEdit>
#include <QList>


class NewGraphWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NewGraphWidget(QWidget *parent = nullptr);
    explicit NewGraphWidget(QWidget *parent = nullptr,QString openCase= "");


private:
    QPushButton* newButton;
    QPushButton* cancelButton;
    QPushButton* openButton;
    QPushButton* removeButton;
    QPushButton* renameButton;
    QPushButton* importButton;

    QLabel* textLabel;
    QTreeWidget* graphTree;
    QListWidget* graphList;
    QLineEdit* lineEdit;
    void initTreeWidget();


    QString getTreeFullPath(QTreeWidgetItem *item);
    void writeFileIntoScene(QString &filePath);
signals:


private slots:
    void on_newButton_clicked();
    void on_graphTree_clicked();
    void on_openButton_clicked();

    void on_importButton_clicked();
    void on_renameButton_clicked();
    void on_removeButton_clicked();
};

#endif // NEWGRAPHWIDGET_H
