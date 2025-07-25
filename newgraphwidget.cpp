#include "newgraphwidget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>

NewGraphWidget::NewGraphWidget(QWidget *parent)
    : QWidget{parent}
{


    setFixedSize(600,400);
    setWindowTitle("新建图");

    newButton = new QPushButton("新建...",this);
    cancelButton = new QPushButton("取消",this);
    textLabel = new QLabel("图名称：",this);
    lineEdit = new QLineEdit(this);
    initTreeWidget();
    graphList = new QListWidget(this);


    QVBoxLayout* mainLayout = new QVBoxLayout(this);



    QHBoxLayout* inputLayout = new QHBoxLayout();
    QHBoxLayout* buttonLayout = new QHBoxLayout();


    QSplitter* graphSpilter = new QSplitter(Qt::Horizontal,this);

    graphSpilter->addWidget(graphTree);
    graphSpilter->addWidget(graphList);
    graphSpilter->setSizes({300, 700});

    inputLayout->addStretch();
    inputLayout->addWidget(textLabel);
    inputLayout->addWidget(lineEdit);


    buttonLayout->addStretch();
    buttonLayout->addWidget(newButton);
    buttonLayout->addWidget(cancelButton);


    mainLayout->addWidget(graphSpilter);
    mainLayout->addLayout(inputLayout);
    mainLayout->addLayout(buttonLayout);



    setLayout(mainLayout);

}

void NewGraphWidget::initTreeWidget()
{
    graphTree = new QTreeWidget();
    graphTree->setColumnCount(1);

    QTreeWidgetItem* graphicsItem  = new QTreeWidgetItem(graphTree);
    graphicsItem ->setText(0,"图形");
    graphicsItem->setFlags(graphicsItem->flags()|Qt::ItemIsSelectable|Qt::ItemIsEnabled);

    QTreeWidgetItem* primitiveItem  = new QTreeWidgetItem(graphTree);
    primitiveItem ->setText(0,"图元");
    primitiveItem->setFlags(primitiveItem->flags()|Qt::ItemIsSelectable|Qt::ItemIsEnabled);


    QTreeWidgetItem* templateItem  = new QTreeWidgetItem(graphTree);
    templateItem ->setText(0,"模板");
    templateItem->setFlags(templateItem->flags()|Qt::ItemIsSelectable|Qt::ItemIsEnabled);


    QTreeWidgetItem* blockItem  = new QTreeWidgetItem(graphTree);
    blockItem ->setText(0,"图块");
    blockItem->setFlags(blockItem->flags()|Qt::ItemIsSelectable|Qt::ItemIsEnabled);

    graphTree->setRootIsDecorated(false);
    graphTree->expandAll();



}



