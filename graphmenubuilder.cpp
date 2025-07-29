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
    fileMenu->addAction("新建",QKeySequence(Qt::CTRL|Qt::Key_N),this,&GraphMenuBuilder::onNewGraphTriggered);
    fileMenu->addAction("打开",QKeySequence(Qt::CTRL|Qt::Key_O),this,&GraphMenuBuilder::onOpenGraphTriggered);
    fileMenu->addAction("保存",QKeySequence(Qt::CTRL|Qt::Key_S),this,&GraphMenuBuilder::onOpenGraphTriggered);//todo
    fileMenu->addAction("导出...",QKeySequence(Qt::CTRL|Qt::Key_E),this,&GraphMenuBuilder::onExportGraphTriggered);
    fileMenu->addAction("导入...",QKeySequence(Qt::CTRL|Qt::Key_I),this,&GraphMenuBuilder::onImportGraphTriggered);
    fileMenu->addSeparator();
    fileMenu->addAction("图管理",QKeySequence(Qt::CTRL|Qt::Key_G),this,&GraphMenuBuilder::onManagerGraphTriggered);
    fileMenu->addAction("符号管理",QKeySequence(Qt::CTRL|Qt::Key_M),this,&GraphMenuBuilder::onManagerGraphTriggered);//todo
    fileMenu->addSeparator();
    fileMenu->addAction("打印",QKeySequence(Qt::CTRL|Qt::Key_P),this,&GraphMenuBuilder::onPrintGraphTriggered);
    fileMenu->addAction("打印预览",this,&GraphMenuBuilder::onPrintGraphTriggered);
    fileMenu->addAction("页面设置",this,&GraphMenuBuilder::onPageSetupGraphTriggered);
    fileMenu->addSeparator();
    fileMenu->addAction("退出",m_parentWindow,&QMainWindow::close);


    QMenu* editMenu = m_menuBar->addMenu("编辑(&F)");
    editMenu->addAction("撤销",QKeySequence(Qt::CTRL|Qt::Key_Z),this,&GraphMenuBuilder::on_actionUndo_triggered);
    editMenu->addAction("重操作",QKeySequence(Qt::CTRL|Qt::Key_Y),this,&GraphMenuBuilder::on_actionRedo_triggered);
    editMenu->addSeparator();
    editMenu->addAction("剪切",QKeySequence(Qt::CTRL|Qt::Key_X),this,&GraphMenuBuilder::on_actionCut_triggered);
    editMenu->addAction("复制",QKeySequence(Qt::CTRL|Qt::Key_C),this,&GraphMenuBuilder::on_actionCopy_triggered);
    editMenu->addAction("粘贴",QKeySequence(Qt::CTRL|Qt::Key_V),this,&GraphMenuBuilder::on_actionPaste_triggered);
    editMenu->addAction("删除",QKeySequence(Qt::Key_Delete),this,&GraphMenuBuilder::on_actionDelete_triggered);
    editMenu->addAction("全选",QKeySequence(Qt::CTRL|Qt::Key_A),this,&GraphMenuBuilder::on_actionSelectAll_triggered);
    editMenu->addSeparator();
    editMenu->addAction("查找替换",QKeySequence(Qt::CTRL|Qt::Key_F),this,&GraphMenuBuilder::on_actionFindReplace_triggered);

    QMenu* viewMenu = m_menuBar->addMenu("视图(&V)");
    viewMenu->addAction("导航图",this,&GraphMenuBuilder::on_actionNavigateGraph_triggered);
    viewMenu->addAction("属性",this,&GraphMenuBuilder::on_actionProperties_triggered);
    viewMenu->addSeparator();
    viewMenu->addAction("标准工具条",this,&GraphMenuBuilder::on_actionStandardToolbar_triggered);
    viewMenu->addAction("图形编辑工具条",this,&GraphMenuBuilder::on_actionGraphEditToolbar_triggered);
    viewMenu->addAction("对齐工具条",this,&GraphMenuBuilder::on_actionAlignmentToolbar_triggered);
    viewMenu->addAction("字体工具条",this,&GraphMenuBuilder::on_actionFontToolbar_triggered);
    viewMenu->addAction("线性工具条",this,&GraphMenuBuilder::on_actionLineToolbar_triggered);
    viewMenu->addAction("图层管理工具条",this,&GraphMenuBuilder::on_actionLayerManagementToolbar_triggered);
    viewMenu->addAction("运行工具条",this,&GraphMenuBuilder::on_actionRunToolbar_triggered);
    viewMenu->addAction("状态条",this,&GraphMenuBuilder::on_actionStatusBar_triggered);
    viewMenu->addSeparator();
    viewMenu->addAction("网格",this,&GraphMenuBuilder::on_actionGrid_triggered);
    viewMenu->addAction("绘图资源管理器",this,&GraphMenuBuilder::on_actionDrawingExplorer_triggered);

    QMenu* toolMenu = m_menuBar->addMenu("工具(&T)");
    toolMenu->addAction("配置预览方式",this,&GraphMenuBuilder::on_actionConfigurePreview_triggered);




}


void GraphMenuBuilder::on_actionUndo_triggered(){}
void GraphMenuBuilder::on_actionRedo_triggered(){}
void GraphMenuBuilder::on_actionCut_triggered(){}
void GraphMenuBuilder::on_actionCopy_triggered(){}
void GraphMenuBuilder::on_actionPaste_triggered(){}
void GraphMenuBuilder::on_actionDelete_triggered(){}
void GraphMenuBuilder::on_actionSelectAll_triggered(){}
void GraphMenuBuilder::on_actionFindReplace_triggered(){}
void GraphMenuBuilder::on_actionConfigurePreview_triggered(){

    m_previewConfigurationDialog = new PreviewConfigurationDialog(m_parentWindow);
    m_previewConfigurationDialog->show();
}

void GraphMenuBuilder::on_actionNavigateGraph_triggered(){}
void GraphMenuBuilder::on_actionProperties_triggered(){}
void GraphMenuBuilder::on_actionStandardToolbar_triggered(){}
void GraphMenuBuilder::on_actionGraphEditToolbar_triggered(){}
void GraphMenuBuilder::on_actionAlignmentToolbar_triggered(){}
void GraphMenuBuilder::on_actionFontToolbar_triggered(){}
void GraphMenuBuilder::on_actionLineToolbar_triggered(){}
void GraphMenuBuilder::on_actionLayerManagementToolbar_triggered(){}
void GraphMenuBuilder::on_actionRunToolbar_triggered(){}
void GraphMenuBuilder::on_actionStatusBar_triggered(){}
void GraphMenuBuilder::on_actionGrid_triggered(){}
void GraphMenuBuilder::on_actionDrawingExplorer_triggered(){}




void GraphMenuBuilder::onPrintGraphTriggered(bool checked ){

    QPrinter printer(QPrinter::HighResolution);
    QPrintPreviewDialog previewDialog(&printer,m_parentWindow);

    connect(&previewDialog,&QPrintPreviewDialog::paintRequested,this,&GraphMenuBuilder::printGraph);
    previewDialog.exec();


}



void GraphMenuBuilder::onPageSetupGraphTriggered(){
    m_pageSetupDialog = new PageSetupDialog(m_parentWindow);
    m_pageSetupDialog->show();

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
