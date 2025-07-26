#include "graphmenubuilder.h"

#include <QFileDialog>
// #include <QtPrintSupport/QPrintDialog>
// #include <QtPrintSupport/QPrinter>
// #include <QtPrintSupport/QPrintPreviewDialog>
#include <QPrintDialog>
#include <QPrinter>
#include <QPrintPreviewDialog>
#include <QPainter> // 打印时通常也需要 QPainter
GraphMenuBuilder::GraphMenuBuilder() {}

GraphMenuBuilder::GraphMenuBuilder(QMainWindow *parent)
{

    m_menuBar = new QMenuBar();
    m_parentWindow = parent;
    m_currentScene = new QGraphicsScene();
    createMenus();
    createGraphActions();
}

QMenuBar *GraphMenuBuilder::buildMenuBar()
{

    return m_menuBar;
}

void GraphMenuBuilder::createMenus()
{
    QMenu* fileMenu = m_menuBar->addMenu("文件(&F)");
    fileMenu->addAction("新建(&N)",this,&GraphMenuBuilder::onNewGraphTriggered);
    fileMenu->addAction("打开(&O)",this,&GraphMenuBuilder::onOpenGraphTriggered);
    fileMenu->addAction("导入(&I)",this,&GraphMenuBuilder::onImportGraphTriggered);
    fileMenu->addAction("导出(&E)",this,&GraphMenuBuilder::onExportGraphTriggered);
    fileMenu->addAction("图管理(&M)",this,&GraphMenuBuilder::onManagerGraphTriggered);
    fileMenu->addAction("打印(&P)",this,&GraphMenuBuilder::onPrintGraphTriggered);

    fileMenu->addAction("退出(&X)",m_parentWindow,&QMainWindow::close);

}
void GraphMenuBuilder::onPrintGraphTriggered(bool checked ){

    QPrinter printer(QPrinter::HighResolution);
    QPrintPreviewDialog previewDialog(&printer,m_parentWindow);

    connect(&previewDialog,&QPrintPreviewDialog::paintRequested,this,&GraphMenuBuilder::printGraph);
    previewDialog.exec();


}

void GraphMenuBuilder::printGraph(){

}
void GraphMenuBuilder::onExportGraphTriggered(bool checked ){
    m_newGraphWidget = new NewGraphWidget(m_parentWindow,"IMPORT");

    qDebug()<<"onExportGraphTriggered ";
    QString filePath = QFileDialog::getSaveFileName(m_parentWindow,"导出图","","Web展示文件(*.nut);;Svg 文件(*.svg);;Xml 文件(*.xml)");
    QFile file(filePath);
    if(file.open(QIODevice::WriteOnly)|QIODevice::Text){
        file.close();
        m_newGraphWidget->writeSceneIntoFile(filePath);

    }


}

void GraphMenuBuilder::createGraphActions()
{

}

void GraphMenuBuilder::onNewGraphTriggered(bool checked ){

    qDebug()<<"onNewGraphTriggered ";
    m_newGraphWidget = new NewGraphWidget(m_parentWindow,"NEW");
    m_newGraphWidget->show();

}


void GraphMenuBuilder::onOpenGraphTriggered(bool checked ){

    qDebug()<<"onOpenGraphTriggered ";
    m_newGraphWidget = new NewGraphWidget(m_parentWindow,"OPEN");
    m_newGraphWidget->show();

}


void GraphMenuBuilder::onImportGraphTriggered(bool checked ){

    qDebug()<<"onImportGraphTriggered ";
    m_newGraphWidget = new NewGraphWidget(m_parentWindow,"IMPORT");
    m_newGraphWidget->show();

}


void GraphMenuBuilder::onManagerGraphTriggered(bool checked ){

    qDebug()<<"onManagerGraphTriggered ";
    m_newGraphWidget = new NewGraphWidget(m_parentWindow,"MANAGER");
    m_newGraphWidget->show();

}
