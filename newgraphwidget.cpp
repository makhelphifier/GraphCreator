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
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    QHBoxLayout* graphLayout = new QHBoxLayout(this);
    QHBoxLayout* inputLayout = new QHBoxLayout(this);
    QHBoxLayout* buttonLayout = new QHBoxLayout(this);


    QSplitter* graphSpilter = new QSplitter(this);

    graphSpilter->addWidget(graphTree);
    graphSpilter->addWidget(graphList);



    graphLayout->addWidget(graphSpilter);

    buttonLayout->addStretch();
    buttonLayout->addWidget(newButton);
    buttonLayout->addWidget(cancelButton);


    mainLayout->addLayout(graphLayout);
    mainLayout->addLayout(inputLayout);
    mainLayout->addLayout(buttonLayout);



    setLayout(mainLayout);

}
