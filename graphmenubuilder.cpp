#include "graphmenubuilder.h"

#include <QFileDialog>
#include <QPrintDialog>
#include <QPrinter>
#include <QPrintPreviewDialog>
#include <QPainter>
#include <QSvgGenerator>
#include <QXmlStreamWriter> // 确保 XML 头文件已包含
#include "enhancedlineitem.h"
#include "enhancedrectangleitem.h"
#include "enhancedellipseitem.h"
#include "enhancedtextitem.h"
#include "enhancedpolylineitem.h"
#include "enhancedpolygonitem.h"
#include "enhancedarcitem.h"

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
    fileMenu->addAction("保存",QKeySequence(Qt::CTRL|Qt::Key_S),this,&GraphMenuBuilder::onSaveGraphTriggered);
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

    QMenu* windowMenu = m_menuBar->addMenu("窗口(&W)");
    windowMenu->addAction("横向平铺",this,&GraphMenuBuilder::on_actionTileHorizontally_triggered);
    windowMenu->addAction("纵向平铺",this,&GraphMenuBuilder::on_actionTileVertically_triggered);
    windowMenu->addAction("层叠",this,&GraphMenuBuilder::on_actionCascade_triggered);
    windowMenu->addAction("排列图标",this,&GraphMenuBuilder::on_actionArrangeIcons_triggered);
    windowMenu->addSeparator();

    QMenu* helpMenu = m_menuBar->addMenu("帮助(&H)");
    helpMenu->addAction("关于",this,&GraphMenuBuilder::on_actionAbout_triggered);
    helpMenu->addAction("更新日志...",this,&GraphMenuBuilder::on_actionChangelog_triggered);
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

void GraphMenuBuilder::on_actionTileHorizontally_triggered() { }
void GraphMenuBuilder::on_actionTileVertically_triggered() { }
void GraphMenuBuilder::on_actionCascade_triggered() { }
void GraphMenuBuilder::on_actionArrangeIcons_triggered() { }
void GraphMenuBuilder::on_actionAbout_triggered() { }
void GraphMenuBuilder::on_actionChangelog_triggered() { }

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
    connect(m_newGraphWidget, &NewGraphWidget::graphCreated, this, &GraphMenuBuilder::onGraphFileCreated);
    m_newGraphWidget->show();
}


void GraphMenuBuilder::onGraphFileCreated(const QString &filePath)
{
    MainWindow* mainWindow = qobject_cast<MainWindow*>(m_parentWindow);
    if (mainWindow) {
        mainWindow->setCurrentFile(filePath);
    }
}


void GraphMenuBuilder::onOpenGraphTriggered(bool checked ){
    qDebug()<<"onOpenGraphTriggered ";
    m_newGraphWidget = new NewGraphWidget(m_parentWindow,"OPEN");
    connect(m_newGraphWidget, &NewGraphWidget::fileOpened, this, &GraphMenuBuilder::openGraph);
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



QString penStyleToString(Qt::PenStyle style);
QString brushStyleToString(Qt::BrushStyle style);


#include "graphmenubuilder.h"
#include "mainwindow.h" // 确保包含了 MainWindow 头文件
#include "enhancedlineitem.h" // 确保包含了直线头文件

#include <QFileDialog>
#include <QXmlStreamWriter> // 使用 QXmlStreamWriter
#include <QDebug>

// ... (其他 include 保持不变)

// --- 辅助函数 ---
// 将 Qt::PenStyle 枚举转换为字符串，用于写入 XML
QString penStyleToString(Qt::PenStyle style) {
    switch (style) {
    case Qt::SolidLine: return "SolidLine";
    case Qt::DashLine: return "DashLine";
    case Qt::DotLine: return "DotLine";
    case Qt::DashDotLine: return "DashDotLine";
    case Qt::DashDotDotLine: return "DashDotDotLine";
    case Qt::CustomDashLine: return "CustomDashLine";
    default: return "NoPen";
    }
}


// --- 重新设计的 onSaveGraphTriggered 函数 ---
void GraphMenuBuilder::onSaveGraphTriggered(bool checked)
{
    Q_UNUSED(checked);

    // 1. 获取 MainWindow 实例
    MainWindow* mainWindow = qobject_cast<MainWindow*>(m_parentWindow);
    if (!mainWindow) {
        qDebug() << "错误: 父窗口不是 MainWindow!";
        return;
    }

    // 2. 获取文件路径，如果为空则弹出“另存为”对话框
    QString currentPath = mainWindow->currentFilePath();
    if (currentPath.isEmpty()) {
        currentPath = QFileDialog::getSaveFileName(
            m_parentWindow,
            "保存为 XML 文件",
            "", // 默认目录
            "Graph Creator XML (*.xml)");

        if (currentPath.isEmpty()) {
            qDebug() << "保存操作已取消";
            return;
        }
        mainWindow->setCurrentFile(currentPath); // 更新主窗口的当前文件路径
    }

    // 3. 打开文件并准备写入
    QFile file(currentPath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "错误: 无法打开文件进行写入: " << currentPath;
        return;
    }

    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true); // 让生成的 XML 文件有缩进，易于阅读
    xmlWriter.writeStartDocument();
    xmlWriter.writeStartElement("graph"); // 根元素

    // 4. 获取场景并保存其基本信息
    QGraphicsScene* scene = mainWindow->scene();
    if (!scene) {
        qDebug() << "错误: 场景为空!";
        file.close();
        return;
    }
    QRectF sceneRect = scene->sceneRect();
    xmlWriter.writeStartElement("scene");
    xmlWriter.writeAttribute("width", QString::number(sceneRect.width()));
    xmlWriter.writeAttribute("height", QString::number(sceneRect.height()));
    xmlWriter.writeEndElement(); // </scene>

    // 5. 遍历场景中的所有图元
    for (QGraphicsItem* item : scene->items()) {

        // --- 核心逻辑：只处理直线 ---
        if (auto line = qgraphicsitem_cast<EnhancedLineItem*>(item)) {
            xmlWriter.writeStartElement("item");

            // a. 写入类型
            xmlWriter.writeAttribute("type", "line");

            // b. 写入通用变换属性
            xmlWriter.writeAttribute("posX", QString::number(line->pos().x()));
            xmlWriter.writeAttribute("posY", QString::number(line->pos().y()));
            xmlWriter.writeAttribute("rotation", QString::number(line->rotation()));
            xmlWriter.writeAttribute("scale", QString::number(line->scale()));
            xmlWriter.writeAttribute("zValue", QString::number(line->zValue()));

            // c. 写入直线专属属性 (端点坐标)
            xmlWriter.writeAttribute("x1", QString::number(line->line().p1().x()));
            xmlWriter.writeAttribute("y1", QString::number(line->line().p1().y()));
            xmlWriter.writeAttribute("x2", QString::number(line->line().p2().x()));
            xmlWriter.writeAttribute("y2", QString::number(line->line().p2().y()));

            // d. 写入画笔属性
            QPen pen = line->pen();
            xmlWriter.writeAttribute("penColor", pen.color().name(QColor::HexArgb)); // 保存为 #AARRGGBB 格式
            xmlWriter.writeAttribute("penWidth", QString::number(pen.width()));
            xmlWriter.writeAttribute("penStyle", penStyleToString(pen.style()));

            xmlWriter.writeEndElement(); // </item>
        }
        else if (auto text = qgraphicsitem_cast<EnhancedTextItem*>(item)) {
            xmlWriter.writeStartElement("item");
            xmlWriter.writeAttribute("type", "text");

            // 通用属性
            xmlWriter.writeAttribute("posX", QString::number(text->pos().x()));
            xmlWriter.writeAttribute("posY", QString::number(text->pos().y()));
            xmlWriter.writeAttribute("rotation", QString::number(text->rotation()));
            xmlWriter.writeAttribute("scale", QString::number(text->scale()));
            xmlWriter.writeAttribute("zValue", QString::number(text->zValue()));

            // 文本专属属性
            xmlWriter.writeAttribute("textWidth", QString::number(text->textWidth()));
            xmlWriter.writeAttribute("font", text->font().toString());
            xmlWriter.writeAttribute("color", text->defaultTextColor().name(QColor::HexArgb));

            // 文本内容
            xmlWriter.writeTextElement("content", text->toPlainText());

            xmlWriter.writeEndElement(); // </item>
        }// --- 新增：处理矩形 ---
        else if (auto rect = qgraphicsitem_cast<EnhancedRectangleItem*>(item)) {
            xmlWriter.writeStartElement("item");
            xmlWriter.writeAttribute("type", "rectangle");

            // 通用属性
            xmlWriter.writeAttribute("posX", QString::number(rect->pos().x()));
            xmlWriter.writeAttribute("posY", QString::number(rect->pos().y()));
            xmlWriter.writeAttribute("rotation", QString::number(rect->rotation()));
            xmlWriter.writeAttribute("scale", QString::number(rect->scale()));
            xmlWriter.writeAttribute("zValue", QString::number(rect->zValue()));

            // 矩形专属属性 (相对于自身坐标系的 x,y,w,h)
            xmlWriter.writeAttribute("x", QString::number(rect->rect().x()));
            xmlWriter.writeAttribute("y", QString::number(rect->rect().y()));
            xmlWriter.writeAttribute("width", QString::number(rect->rect().width()));
            xmlWriter.writeAttribute("height", QString::number(rect->rect().height()));

            // 画笔和画刷属性
            QPen pen = rect->pen();
            QBrush brush = rect->brush();
            xmlWriter.writeAttribute("penColor", pen.color().name(QColor::HexArgb));
            xmlWriter.writeAttribute("penWidth", QString::number(pen.width()));
            xmlWriter.writeAttribute("penStyle", penStyleToString(pen.style()));
            xmlWriter.writeAttribute("brushColor", brush.color().name(QColor::HexArgb));
            xmlWriter.writeAttribute("brushStyle", brushStyleToString(brush.style()));

            xmlWriter.writeEndElement(); // </item>
        }// --- 新增：处理椭圆 ---
        else if (auto ellipse = qgraphicsitem_cast<EnhancedEllipseItem*>(item)) {
            xmlWriter.writeStartElement("item");
            xmlWriter.writeAttribute("type", "ellipse");

            // 通用属性
            xmlWriter.writeAttribute("posX", QString::number(ellipse->pos().x()));
            xmlWriter.writeAttribute("posY", QString::number(ellipse->pos().y()));
            xmlWriter.writeAttribute("rotation", QString::number(ellipse->rotation()));
            xmlWriter.writeAttribute("scale", QString::number(ellipse->scale()));
            xmlWriter.writeAttribute("zValue", QString::number(ellipse->zValue()));

            // 椭圆专属属性 (外接矩形的 x,y,w,h)
            xmlWriter.writeAttribute("x", QString::number(ellipse->rect().x()));
            xmlWriter.writeAttribute("y", QString::number(ellipse->rect().y()));
            xmlWriter.writeAttribute("width", QString::number(ellipse->rect().width()));
            xmlWriter.writeAttribute("height", QString::number(ellipse->rect().height()));

            // 画笔和画刷属性
            QPen pen = ellipse->pen();
            QBrush brush = ellipse->brush();
            xmlWriter.writeAttribute("penColor", pen.color().name(QColor::HexArgb));
            xmlWriter.writeAttribute("penWidth", QString::number(pen.width()));
            xmlWriter.writeAttribute("penStyle", penStyleToString(pen.style()));
            xmlWriter.writeAttribute("brushColor", brush.color().name(QColor::HexArgb));
            xmlWriter.writeAttribute("brushStyle", brushStyleToString(brush.style()));

            xmlWriter.writeEndElement(); // </item>
        }
        // --- 新增：处理圆弧 ---
        else if (auto arc = qgraphicsitem_cast<EnhancedArcItem*>(item)) {
            xmlWriter.writeStartElement("item");
            xmlWriter.writeAttribute("type", "arc");

            // 通用属性 (pos 就是圆心)
            xmlWriter.writeAttribute("posX", QString::number(arc->pos().x()));
            xmlWriter.writeAttribute("posY", QString::number(arc->pos().y()));
            xmlWriter.writeAttribute("rotation", QString::number(arc->rotation()));
            xmlWriter.writeAttribute("scale", QString::number(arc->scale()));
            xmlWriter.writeAttribute("zValue", QString::number(arc->zValue()));

            // 圆弧专属属性 (起点和终点，相对于圆心)
            xmlWriter.writeAttribute("startX", QString::number(arc->startPoint().x()));
            xmlWriter.writeAttribute("startY", QString::number(arc->startPoint().y()));
            xmlWriter.writeAttribute("endX", QString::number(arc->endPoint().x()));
            xmlWriter.writeAttribute("endY", QString::number(arc->endPoint().y()));

            // 画笔和画刷属性
            QPen pen = arc->pen();
            QBrush brush = arc->brush();
            xmlWriter.writeAttribute("penColor", pen.color().name(QColor::HexArgb));
            xmlWriter.writeAttribute("penWidth", QString::number(pen.width()));
            xmlWriter.writeAttribute("penStyle", penStyleToString(pen.style()));
            xmlWriter.writeAttribute("brushColor", brush.color().name(QColor::HexArgb));
            xmlWriter.writeAttribute("brushStyle", brushStyleToString(brush.style()));

            xmlWriter.writeEndElement(); // </item>
        }// --- 新增：处理多边形 ---
        else if (auto polygon = qgraphicsitem_cast<EnhancedPolygonItem*>(item)) {
            xmlWriter.writeStartElement("item");
            xmlWriter.writeAttribute("type", "polygon");

            // 通用属性
            xmlWriter.writeAttribute("posX", QString::number(polygon->pos().x()));
            xmlWriter.writeAttribute("posY", QString::number(polygon->pos().y()));
            xmlWriter.writeAttribute("rotation", QString::number(polygon->rotation()));
            xmlWriter.writeAttribute("scale", QString::number(polygon->scale()));
            xmlWriter.writeAttribute("zValue", QString::number(polygon->zValue()));

            // 多边形专属属性 (点集合)
            QPainterPath path = polygon->path();
            QString pointsStr;
            // 遍历路径中的所有点
            for (int i = 0; i < path.elementCount(); ++i) {
                const QPainterPath::Element &element = path.elementAt(i);
                // 忽略路径中的非 "LineTo" 和 "MoveTo" 元素
                if (element.isMoveTo() || element.isLineTo()) {
                    pointsStr += QString("%1,%2;").arg(element.x).arg(element.y);
                }
            }
            if (!pointsStr.isEmpty()) {
                pointsStr.chop(1); // 移除最后一个分号
            }
            xmlWriter.writeAttribute("points", pointsStr);

            // 画笔和画刷属性
            QPen pen = polygon->pen();
            QBrush brush = polygon->brush();
            xmlWriter.writeAttribute("penColor", pen.color().name(QColor::HexArgb));
            xmlWriter.writeAttribute("penWidth", QString::number(pen.width()));
            xmlWriter.writeAttribute("penStyle", penStyleToString(pen.style()));
            xmlWriter.writeAttribute("brushColor", brush.color().name(QColor::HexArgb));
            xmlWriter.writeAttribute("brushStyle", brushStyleToString(brush.style()));

            xmlWriter.writeEndElement(); // </item>
        }// --- 新增：处理多线段 ---
        else if (auto polyline = qgraphicsitem_cast<EnhancedPolylineItem*>(item)) {
            xmlWriter.writeStartElement("item");
            xmlWriter.writeAttribute("type", "polyline");

            // 通用属性
            xmlWriter.writeAttribute("posX", QString::number(polyline->pos().x()));
            xmlWriter.writeAttribute("posY", QString::number(polyline->pos().y()));
            xmlWriter.writeAttribute("rotation", QString::number(polyline->rotation()));
            xmlWriter.writeAttribute("scale", QString::number(polyline->scale()));
            xmlWriter.writeAttribute("zValue", QString::number(polyline->zValue()));

            // 多线段专属属性 (点集合)
            QPainterPath path = polyline->path();
            QString pointsStr;
            // 遍历路径中的所有元素（点）
            for (int i = 0; i < path.elementCount(); ++i) {
                const QPainterPath::Element &element = path.elementAt(i);
                // 将每个点的 x 和 y 坐标拼接成 "x,y;" 的格式
                pointsStr += QString("%1,%2;").arg(element.x).arg(element.y);
            }
            // 移除最后一个多余的分号
            if (!pointsStr.isEmpty()) {
                pointsStr.chop(1);
            }
            xmlWriter.writeAttribute("points", pointsStr);

            // 画笔属性 (多线段没有填充)
            QPen pen = polyline->pen();
            xmlWriter.writeAttribute("penColor", pen.color().name(QColor::HexArgb));
            xmlWriter.writeAttribute("penWidth", QString::number(pen.width()));
            xmlWriter.writeAttribute("penStyle", penStyleToString(pen.style()));

            xmlWriter.writeEndElement(); // </item>
        }
    }

    // 6. 结束写入并关闭文件
    xmlWriter.writeEndElement(); // </graph>
    xmlWriter.writeEndDocument();
    file.close();

    qDebug() << "场景已成功保存到:" << currentPath;
}



// 将 Qt::BrushStyle 枚举转换为字符串
QString brushStyleToString(Qt::BrushStyle style) {
    switch (style) {
    case Qt::SolidPattern: return "SolidPattern";
    case Qt::Dense1Pattern: return "Dense1Pattern";
    case Qt::Dense2Pattern: return "Dense2Pattern";
    case Qt::Dense3Pattern: return "Dense3Pattern";
    case Qt::Dense4Pattern: return "Dense4Pattern";
    case Qt::Dense5Pattern: return "Dense5Pattern";
    case Qt::Dense6Pattern: return "Dense6Pattern";
    case Qt::HorPattern: return "HorPattern";
    case Qt::VerPattern: return "VerPattern";
    case Qt::CrossPattern: return "CrossPattern";
    case Qt::BDiagPattern: return "BDiagPattern";
    case Qt::FDiagPattern: return "FDiagPattern";
    case Qt::DiagCrossPattern: return "DiagCrossPattern";
    default: return "NoBrush";
    }
}
