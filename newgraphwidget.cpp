#include "newgraphwidget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <qheaderview.h>
#include <QMessageBox>
#include <QStandardPaths>
#include <QDir>
#include <QFileDialog>
#include <QGraphicsRectItem>

#include "enhancedlineitem.h"
#include "enhancedpolylineitem.h"
#include <QPainterPath>
#include "enhancedrectangleitem.h"
#include <QGraphicsLineItem>
#include <QPen>
#include "enhancedtextitem.h"
#include "enhancedellipseitem.h"
#include "enhancedarcitem.h"
#include "enhancedpieitem.h"
#include "enhancedpolygonitem.h"

NewGraphWidget::NewGraphWidget(QWidget *parent)
    : QWidget{parent}
{
    setFixedSize(600,400);
    setWindowTitle("新建图");
    setWindowIcon(QIcon());

    newButton = new QPushButton("新建...",this);
    cancelButton = new QPushButton("取消",this);
    textLabel = new QLabel("图名称：",this);
    lineEdit = new QLineEdit(this);
    initTreeWidget();
    graphList = new QListWidget();
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


    connect(newButton,&QPushButton::clicked,this,&NewGraphWidget::on_newButton_clicked);
    connect(cancelButton,&QPushButton::clicked,this,&NewGraphWidget::close);

    connect(graphTree,&QTreeWidget::itemClicked,this,&NewGraphWidget::on_graphTree_clicked);
}

NewGraphWidget::NewGraphWidget(QWidget *parent, QString openCase)
{

    setFixedSize(600,400);
    setWindowIcon(QIcon());



    textLabel = new QLabel("图名称：",this);
    lineEdit = new QLineEdit(this);
    // m_finalDirPath ="";
    initTreeWidget();
    graphList = new QListWidget();
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

    if(openCase=="OPEN"){
        setWindowTitle("打开图");
        openButton = new QPushButton("打开",this);
        buttonLayout->addWidget(openButton);
        connect(openButton,&QPushButton::clicked,this,&NewGraphWidget::on_openButton_clicked);

    }else if(openCase=="MANAGER") {
        setWindowTitle("图管理");
        removeButton = new QPushButton("删除",this);
        renameButton = new QPushButton("重命名",this);
        buttonLayout->addWidget(renameButton);
        buttonLayout->addWidget(removeButton);
        connect(removeButton,&QPushButton::clicked,this,&NewGraphWidget::on_removeButton_clicked);
        connect(renameButton,&QPushButton::clicked,this,&NewGraphWidget::on_renameButton_clicked);


    }else if(openCase=="NEW") {
        setWindowTitle("新建图");
        newButton = new QPushButton("新建...",this);
        buttonLayout->addWidget(newButton);
        connect(newButton,&QPushButton::clicked,this,&NewGraphWidget::on_newButton_clicked);

    }else if(openCase=="IMPORT") {
        setWindowTitle("导入图");
        importButton = new QPushButton("选择文件",this);
        buttonLayout->addWidget(importButton);
        connect(importButton,&QPushButton::clicked,this,&NewGraphWidget::on_importButton_clicked);

    }
    cancelButton = new QPushButton("取消",this);
    buttonLayout->addWidget(cancelButton);



    mainLayout->addWidget(graphSpilter);
    mainLayout->addLayout(inputLayout);
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);


    connect(cancelButton,&QPushButton::clicked,this,&NewGraphWidget::close);

    connect(graphTree,&QTreeWidget::itemClicked,this,&NewGraphWidget::on_graphTree_clicked);


}

void NewGraphWidget::writeSceneIntoFile(QString &filePath)
{

}

void NewGraphWidget::on_renameButton_clicked(){
    QString targetFileName = lineEdit->text();
    if(targetFileName.isEmpty()){
        return;
    }
    QTreeWidgetItem* selectedItem = graphTree->currentItem();
    if(!selectedItem){
        qDebug()<<"未选中";
        return;
    }
    if(!graphList->currentItem()){
        return ;
    }
    QString documentPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QString defaultSavePath = QDir(documentPath).filePath("GraphCreator");
    QString treePath = getTreeFullPath(selectedItem);
    QString finalDirPath = defaultSavePath.append(QDir::separator()).append(treePath);
    QString  targetPath = finalDirPath+( QDir::separator())+(targetFileName)+(".xml");
    QString  sourcePath = finalDirPath+( QDir::separator())+(graphList->currentItem()->text());

    QFile::remove(targetPath);
    QFile::copy(sourcePath, targetPath);
    QFile::remove(sourcePath);
    update();
    on_graphTree_clicked();
}
void NewGraphWidget::on_removeButton_clicked(){
    QString targetFileName = lineEdit->text();
    if(targetFileName.isEmpty()){
        return;
    }
    QTreeWidgetItem* selectedItem = graphTree->currentItem();
    if(!selectedItem){
        qDebug()<<"未选中";
        return;
    }
    if(!graphList->currentItem()){
        return ;
    }
    QString documentPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QString defaultSavePath = QDir(documentPath).filePath("GraphCreator");
    QString treePath = getTreeFullPath(selectedItem);
    QString finalDirPath = defaultSavePath.append(QDir::separator()).append(treePath);
    QString  sourcePath = finalDirPath.append( QDir::separator()).append(graphList->currentItem()->text());

    QFile::remove(sourcePath);
    update();
    on_graphTree_clicked();

}



void NewGraphWidget::on_importButton_clicked()
{
    QTreeWidgetItem* selectedItem = graphTree->currentItem();
    if(!selectedItem){
        qDebug()<<"未选中";
        return;
    }
    QString documentPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QString filePath = QFileDialog::getOpenFileName(this,"选择导入的文件",documentPath,"XML Files (*.xml)");

    QFileInfo fileInfo(filePath);
    QString fileName = fileInfo.fileName();
    QString defaultSavePath = QDir(documentPath).filePath("GraphCreator");
    QString treePath = getTreeFullPath(selectedItem);
    QString finalDirPath = defaultSavePath.append(QDir::separator()).append(treePath);
    QString  targetPath = finalDirPath.append( QDir::separator()).append(fileName);
    QFile::remove(targetPath);
    QFile::copy(filePath, targetPath);
    // writeFileIntoScene(targetPath);

    qDebug()<<"导入文件"+targetPath;
    this->close();

}


void NewGraphWidget::on_openButton_clicked()
{
    QTreeWidgetItem* selectedItem = graphTree->currentItem();
    if(!selectedItem){
        qDebug()<<"未选中";
        return;
    }
    if(!graphList->currentItem()){
        return ;
    }
    QString documentPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QString defaultSavePath = QDir(documentPath).filePath("GraphCreator");
    QString treePath = getTreeFullPath(selectedItem);
    QString finalDirPath = defaultSavePath.append(QDir::separator()).append(treePath);
    QString filePath = finalDirPath.append( QDir::separator()).append( graphList->currentItem()->text());

    // writeFileIntoScene(filePath);
    qDebug()<<"打开文件"+filePath;
    this->close();
}


void NewGraphWidget::on_graphTree_clicked(){
    qDebug()<<"on_graphTree_clicked";
    graphList->clear();
    update();

    QTreeWidgetItem* selectedItem = graphTree->currentItem();
    if(!selectedItem){
        qDebug()<<"未选中";
        return;
    }
    QString documentPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QString defaultSavePath = QDir(documentPath).filePath("GraphCreator");
    QString treePath = getTreeFullPath(selectedItem);
    QString finalDirPath = defaultSavePath.append(QDir::separator()).append(treePath);
    // m_finalDirPath = finalDirPath;
    QDir dir(finalDirPath) ;
    if(!dir.exists()){
        qDebug()<<"路径不存在";

        return;
    }

    QStringList fileList = dir.entryList(QDir::Files|QDir::NoDotAndDotDot);
    qDebug()<<"fileList";
    for(const QString &fileName:fileList){
        qDebug()<<fileName;
        graphList->addItem(fileName);
    }
    update();

}




void NewGraphWidget::on_newButton_clicked(){

    QString graphName =  lineEdit->text().trimmed();
    if(graphName.isEmpty()){
        QMessageBox::warning(this,"警告","图名称不能为空，请输入");
        return;
    }

    QTreeWidgetItem* selectedItem = graphTree->currentItem();
    if(!selectedItem){
        QMessageBox::warning(this,"警告","请在左侧树状图选择");
    }

    QString documentPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QString defaultSavePath = QDir(documentPath).filePath("GraphCreator");
    QString treePath = getTreeFullPath(selectedItem);
    QString finalDirPath = defaultSavePath.append(QDir::separator()).append(treePath);

    QDir dir;
    if(!dir.mkpath(finalDirPath)){
        QMessageBox::critical(this,"错误","无法创建目录"+defaultSavePath.append(QDir::separator()).append(treePath)+"\n请检查程序权限或目标路径是否有效。");
    }


    QString filePath = finalDirPath.append(QDir::separator()).append(graphName).append(".xml");
    filePath = QDir::cleanPath(filePath);
    QFile file(filePath);

    if(file.open(QIODevice::WriteOnly)|QIODevice::Text){

        QTextStream out(&file);
        out<<"";
        out.flush();
        file.close();
        qDebug()<<filePath<<"文件已经成功创建";
        // writeFileIntoScene(filePath);
        emit graphCreated(filePath);

        this->close();
    }


}


void NewGraphWidget::initTreeWidget()
{
    graphTree = new QTreeWidget();
    graphTree->setColumnCount(1);

    QTreeWidgetItem* graphicsItem  = new QTreeWidgetItem(graphTree);
    graphicsItem ->setText(0,"图形");
    graphicsItem->setFlags(graphicsItem->flags()|Qt::ItemIsSelectable|Qt::ItemIsEnabled);

    QTreeWidgetItem* publicItem = new QTreeWidgetItem(graphicsItem);
    publicItem->setText(0,"公用");
    publicItem->setFlags(publicItem->flags()|Qt::ItemIsSelectable|Qt::ItemIsEnabled);


    QTreeWidgetItem* jiangsuItem = new QTreeWidgetItem(graphicsItem);
    jiangsuItem->setText(0,"江苏");
    jiangsuItem->setFlags(jiangsuItem->flags()|Qt::ItemIsSelectable|Qt::ItemIsEnabled);


    QTreeWidgetItem* jiangsu5Item = new QTreeWidgetItem(jiangsuItem);
    jiangsu5Item->setText(0,"5#");
    jiangsu5Item->setFlags(jiangsu5Item->flags()|Qt::ItemIsSelectable|Qt::ItemIsEnabled);


    QTreeWidgetItem* jiangsu6Item = new QTreeWidgetItem(jiangsuItem);
    jiangsu6Item->setText(0,"6#");
    jiangsu6Item->setFlags(jiangsu6Item->flags()|Qt::ItemIsSelectable|Qt::ItemIsEnabled);

    QTreeWidgetItem* primitiveItem  = new QTreeWidgetItem(graphTree);
    primitiveItem ->setText(0,"图元");
    primitiveItem->setFlags(primitiveItem->flags()|Qt::ItemIsSelectable|Qt::ItemIsEnabled);

    QTreeWidgetItem* primitiveCartoonItem  = new QTreeWidgetItem(primitiveItem);
    primitiveCartoonItem ->setText(0,"动画元素");
    primitiveCartoonItem->setFlags(primitiveCartoonItem->flags()|Qt::ItemIsSelectable|Qt::ItemIsEnabled);


    QTreeWidgetItem* primitivePublicItem  = new QTreeWidgetItem(primitiveItem);
    primitivePublicItem ->setText(0,"公用");
    primitivePublicItem->setFlags(primitivePublicItem->flags()|Qt::ItemIsSelectable|Qt::ItemIsEnabled);



    QTreeWidgetItem* templateItem  = new QTreeWidgetItem(graphTree);
    templateItem ->setText(0,"模板");
    templateItem->setFlags(templateItem->flags()|Qt::ItemIsSelectable|Qt::ItemIsEnabled);


    QTreeWidgetItem* templatePublicItem  = new QTreeWidgetItem(templateItem);
    templatePublicItem ->setText(0,"公用模板");
    templatePublicItem->setFlags(templatePublicItem->flags()|Qt::ItemIsSelectable|Qt::ItemIsEnabled);


    QTreeWidgetItem* blockItem  = new QTreeWidgetItem(graphTree);
    blockItem ->setText(0,"图块");
    blockItem->setFlags(blockItem->flags()|Qt::ItemIsSelectable|Qt::ItemIsEnabled);


    QTreeWidgetItem* blockPublicItem  = new QTreeWidgetItem(blockItem);
    blockPublicItem ->setText(0,"公用图块");
    blockPublicItem->setFlags(blockPublicItem->flags()|Qt::ItemIsSelectable|Qt::ItemIsEnabled);


    graphTree->setRootIsDecorated(false);
    graphTree->expandAll();
    graphTree->header()->hide();


}

QString NewGraphWidget::getTreeFullPath(QTreeWidgetItem* item){

    QStringList pathList;

    while(item){
        pathList.prepend(item->text(0));
        item = item->parent();
    }
    return pathList.join(QDir::separator());
}



void NewGraphWidget::writeFileIntoScene(const QString &filePath,QGraphicsScene* scene)
{

    // QGraphicsRectItem* item = new QGraphicsRectItem(3,4,12,23);
    // QGraphicsLineItem* line = new QGraphicsLineItem(100,100,200,200);
    // QPen pen ;
    // pen.setColor(Qt::red);
    // line->setPen(pen);
    // line->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);

    // item->setBrush(Qt::yellow);
    // scene->addItem(item);
    // scene->addItem(line);

    // GraphTextItem* text = new GraphTextItem("测试",nullptr);
    // scene->addItem(text);

    // EnhancedLineItem* line = new EnhancedLineItem();
    // line->setLine(QLineF(QPointF(20,20),QPointF(100,100)));
    // scene->addItem(line);

    // EnhancedPolylineItem* polyline = new EnhancedPolylineItem();
    // QPainterPath path ;
    // path.moveTo(10,20);
    // path.lineTo(40,80);
    // path.lineTo(100,100);
    // polyline->setPath(path);
    // scene->addItem(polyline);

    // EnhancedRectangleItem* rec = new EnhancedRectangleItem();
    // rec->setRect(QRectF(QPointF(50,50),QSizeF(50,50)));
    // scene->addItem(rec);


    // EnhancedEllipseItem* ellipse = new EnhancedEllipseItem();
    // ellipse->setRect(QRectF(QPointF(50,50),QSizeF(50,50)));
    // scene->addItem(ellipse);

    // EnhancedArcItem* arc= new EnhancedArcItem();
    // scene->addItem(arc);

    // // 2. 定义三个点 (场景坐标)
    // QPointF center(50, 50);
    // QPointF startPoint(250, 50);       // 3点钟方向
    // QPointF endPoint(150, -173.2+50);    // 11点钟方向 (cos(300), sin(300))

    // // 3. 创建我们的自定义 PieItem
    // EnhancedArcItem *pie = new EnhancedArcItem(center, startPoint, endPoint);

    // // 4. 将 item 添加到场景中
    // scene->addItem(pie);


    // 创建一个简单的三角形路径
    // QPainterPath path;
    // path.moveTo(0, 0);           // 起点
    // path.lineTo(100, 0);         // 第二个点
    // path.lineTo(50, 100);        // 第三个点
    // path.lineTo(100, 100);        // 第三个点
    // path.closeSubpath();         // 闭合路径，形成三角形

    // // 创建 EnhancedPolygonItem 并设置路径
    // EnhancedPolygonItem *polygon = new EnhancedPolygonItem();
    // polygon->setPath(path);

    // // 将 EnhancedPolygonItem 添加到场景
    // scene->addItem(polygon);

}

