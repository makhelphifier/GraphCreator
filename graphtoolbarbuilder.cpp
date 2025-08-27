#include "graphtoolbarbuilder.h"
#include <QMainWindow>
#include <QMenu>
#include <QWidgetAction>
#include <QGridLayout>
#include <QPushButton>
#include <QColorDialog>
#include <QGuiApplication>
#include <QScreen>
#include <QMouseEvent>
#include <QApplication>
#include "brusheditordialog.h"

GraphToolBarBuilder::GraphToolBarBuilder(QMainWindow *parent)

{
    m_parentWindow = parent;
    m_toolBar =new QToolBar(parent);
    createMenus();
    m_isPickingColor = false;
    m_customColorIndex = 0;

}

QToolBar *GraphToolBarBuilder::buildToolBar()
{
    return m_toolBar;
}



// 2. 事件过滤器，用于捕获全局鼠标点击
bool GraphToolBarBuilder::eventFilter(QObject *watched, QEvent *event)
{
    // 只在取色模式下进行处理
    if (m_isPickingColor) {
        // 如果是鼠标左键按下的事件
        if (event->type() == QEvent::MouseButtonPress) {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            if (mouseEvent->button() == Qt::LeftButton) {
                // 获取鼠标点击的全局坐标
                QPoint pos = QCursor::pos();
                // 抓取屏幕上这个坐标的1x1像素的图像
                QPixmap pixmap = QGuiApplication::primaryScreen()->grabWindow(0, pos.x(), pos.y(), 1, 1);
                // 从图像中获取颜色
                QColor pickedColor = pixmap.toImage().pixelColor(0, 0);
                qDebug()<<pickedColor.name();
                // --- 恢复状态 ---
                QApplication::restoreOverrideCursor(); // 恢复鼠标光标
                m_isPickingColor = false;
                qApp->removeEventFilter(this);

                // --- 更新自定义颜色格子 ---
                if (m_customColorIndex < m_customColorButtons.size()) {
                    QPushButton* btn = m_customColorButtons[m_customColorIndex];
                    QString style = QString("background-color: %1; border: 1px solid gray;").arg(pickedColor.name());
                    btn->setStyleSheet(style);
                    // 索引+1，如果超过7就回到0，实现循环填充
                    m_customColorIndex = (m_customColorIndex + 1) % m_customColorButtons.size();
                }

                // 返回true表示事件已被处理，不需要再传递
                return true;
            }
        }
        // 如果是按键事件，并且是Esc键，则取消取色
        else if (event->type() == QEvent::KeyPress) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
            if (keyEvent->key() == Qt::Key_Escape) {
                QApplication::restoreOverrideCursor();
                m_isPickingColor = false;
                qApp->removeEventFilter(this);

                return true;
            }
        }
    }

    // 对于其他所有事件，返回默认处理方式
    return QObject::eventFilter(watched, event);
}

void GraphToolBarBuilder::createMenus()
{

    connect(m_toolBar->addAction("文字"),&QAction::triggered,this,&GraphToolBarBuilder::onTextTriggered);
    connect(m_toolBar->addAction("线段"),&QAction::triggered,this,&GraphToolBarBuilder::onLineTriggered);
    connect(m_toolBar->addAction("多线段"),&QAction::triggered,this,&GraphToolBarBuilder::onPolylineTriggered);
    connect(m_toolBar->addAction("矩形"),&QAction::triggered,this,&GraphToolBarBuilder::onRectangleTriggered);
    connect(m_toolBar->addAction("椭圆"),&QAction::triggered,this,&GraphToolBarBuilder::onEclipseTriggered);
    connect(m_toolBar->addAction("圆弧"),&QAction::triggered,this,&GraphToolBarBuilder::onArcTriggered);
    connect(m_toolBar->addAction("多边形"),&QAction::triggered,this,&GraphToolBarBuilder::onPolygonTriggered);
    m_toolBar->addSeparator();
    createFillColorMenu();
    createPenMenu();

}
void GraphToolBarBuilder::createFillColorMenu()
{
    QMenu* fillColorMenu = new QMenu(m_parentWindow);
    fillColorMenu->addAction("无填充颜色");
    QWidgetAction* colorGridAction = new QWidgetAction(fillColorMenu);
    QWidgetAction* colorGridAction2 = new QWidgetAction(fillColorMenu);
    QWidget* colorWidget = new QWidget();
    QWidget* customColorWidget  = new QWidget();
    QGridLayout* gridLayout = new QGridLayout(colorWidget);
    QGridLayout* customLayout  = new QGridLayout(customColorWidget );
    gridLayout->setSpacing(5);
    gridLayout->setContentsMargins(5,5,5,5);
    customLayout ->setSpacing(5);
    customLayout ->setContentsMargins(5,5,5,5);
    QColor colors[5][8] = {
        {Qt::black, QColor(139,69,19), QColor(85,107,47), QColor(0,100,0), QColor(0,0,128), QColor(25,25,112), QColor(72,61,139), QColor(105,105,105)},
        {Qt::darkRed, Qt::white, QColor(189,183,107), Qt::darkGreen, Qt::darkBlue, QColor(138,43,226), QColor(128,0,128), Qt::darkGray},
        {Qt::red, Qt::white, Qt::yellow, Qt::green, Qt::blue, Qt::cyan, QColor(148,0,211), Qt::gray},
        {Qt::magenta, QColor(255,255,0), QColor(0,255,255), QColor(0,191,255), QColor(0,0,255), QColor(255,0,255), QColor(169,169,169), Qt::white},
        {QColor(255,192,203), QColor(255,228,196), QColor(240,230,140), QColor(152,251,152), QColor(176,224,230), QColor(135,206,250), QColor(221,160,221), QColor(255,255,255)}
    };
    // 动态创建颜色按钮，并添加到网格布局中
    for (int row = 0; row < 5; ++row) {
        for (int col = 0; col < 8; ++col) {
            QPushButton *colorButton = new QPushButton();
            // 设置按钮的固定大小，使其成为一个正方形
            colorButton->setFixedSize(15, 15);
            // 设置按钮背景颜色，并添加边框
            QString styleSheet = QString("background-color: %1; border: 1px solid gray;").arg(colors[row][col].name());
            colorButton->setStyleSheet(styleSheet);

            // 连接按钮点击信号，使用lambda表达式传递颜色
            connect(colorButton, &QPushButton::clicked, this, [=]() {
                qDebug() << "选择了颜色: " << colors[row][col].name();
                // 可以在这里发出信号，通知主窗口改变画笔颜色
                fillColorMenu->close(); // 点击颜色后关闭菜单
            });

            gridLayout->addWidget(colorButton, row, col);

        }
    }
    colorGridAction->setDefaultWidget(colorWidget);
    fillColorMenu->addAction(colorGridAction);
    fillColorMenu->addSeparator();


    // 找到这一行
    QAction* otherColorAction  = fillColorMenu->addAction("其他填充颜色");

    // 把它的 connect 调用从 QColorDialog 改成我们的新对话框
    connect(otherColorAction, &QAction::triggered, this, [=](){
        // 创建我们的画刷编辑器对话框
        BrushEditorDialog dialog(m_parentWindow);

        // 以模态方式显示对话框，并判断用户是否点击了“确定”
        if (dialog.exec() == QDialog::Accepted) {
            // 如果用户点了“确定”，就获取配置好的画刷
            QBrush selectedBrush = dialog.getBrush();

            // TODO: 发射一个携带QBrush的新信号，通知主窗口填充画刷已改变
            qDebug() << "新的画刷已选择!";
            // 比如: emit fillColorChanged(selectedBrush);
        }
    });
    BrushEditorDialog dialog(m_parentWindow);
    dialog.exec();
    QAction* pickColorAction = fillColorMenu->addAction("取色");
    QColor colors2[1][8] = {
        {Qt::white,Qt::white,Qt::white,Qt::white,Qt::white,Qt::white,Qt::white,Qt::white,}
    };

    // 动态创建颜色按钮，并添加到网格布局中
    for (int row = 0; row < 1; ++row) {
        for (int col = 0; col < 8; ++col) {
            QPushButton *colorButton = new QPushButton();
            // 设置按钮的固定大小，使其成为一个正方形
            colorButton->setFixedSize(15, 15);
            // 设置按钮背景颜色，并添加边框
            QString styleSheet = QString("background-color: %1; border: 1px solid gray;").arg(colors2[row][col].name());
            colorButton->setStyleSheet(styleSheet);

            // 连接按钮点击信号，使用lambda表达式传递颜色
            connect(colorButton, &QPushButton::clicked, this, [=]() {
                qDebug() << "选择了颜色: " << colors2[row][col].name();
                // 可以在这里发出信号，通知主窗口改变画笔颜色
                fillColorMenu->close(); // 点击颜色后关闭菜单
            });

            customLayout ->addWidget(colorButton, row, col);
            m_customColorButtons.append(colorButton); // 将按钮指针存起来，方便后续更新颜色

        }
    }
    colorGridAction2->setDefaultWidget(customColorWidget);
    fillColorMenu->addAction(colorGridAction2);

    connect(pickColorAction,&QAction::triggered,this,&GraphToolBarBuilder::onPickColorTriggered );
    QAction* penAction = new QAction("填充颜色",m_parentWindow);
    penAction->setMenu(fillColorMenu);
    m_toolBar->addAction(penAction);
}

void GraphToolBarBuilder::createPenMenu()
{
    QMenu* penMenu = new QMenu(m_parentWindow);
    penMenu->addAction("无线条颜色");
    QWidgetAction* colorGridAction = new QWidgetAction(penMenu);
    QWidgetAction* colorGridAction2 = new QWidgetAction(penMenu);
    QWidget* colorWidget = new QWidget();
    QWidget* customColorWidget  = new QWidget();
    QGridLayout* gridLayout = new QGridLayout(colorWidget);
    QGridLayout* customLayout  = new QGridLayout(customColorWidget );
    gridLayout->setSpacing(5);
    gridLayout->setContentsMargins(5,5,5,5);
    customLayout ->setSpacing(5);
    customLayout ->setContentsMargins(5,5,5,5);
    QColor colors[5][8] = {
        {Qt::black, QColor(139,69,19), QColor(85,107,47), QColor(0,100,0), QColor(0,0,128), QColor(25,25,112), QColor(72,61,139), QColor(105,105,105)},
        {Qt::darkRed, Qt::white, QColor(189,183,107), Qt::darkGreen, Qt::darkBlue, QColor(138,43,226), QColor(128,0,128), Qt::darkGray},
        {Qt::red, Qt::white, Qt::yellow, Qt::green, Qt::blue, Qt::cyan, QColor(148,0,211), Qt::gray},
        {Qt::magenta, QColor(255,255,0), QColor(0,255,255), QColor(0,191,255), QColor(0,0,255), QColor(255,0,255), QColor(169,169,169), Qt::white},
        {QColor(255,192,203), QColor(255,228,196), QColor(240,230,140), QColor(152,251,152), QColor(176,224,230), QColor(135,206,250), QColor(221,160,221), QColor(255,255,255)}
    };
    // 动态创建颜色按钮，并添加到网格布局中
    for (int row = 0; row < 5; ++row) {
        for (int col = 0; col < 8; ++col) {
            QPushButton *colorButton = new QPushButton();
            // 设置按钮的固定大小，使其成为一个正方形
            colorButton->setFixedSize(15, 15);
            // 设置按钮背景颜色，并添加边框
            QString styleSheet = QString("background-color: %1; border: 1px solid gray;").arg(colors[row][col].name());
            colorButton->setStyleSheet(styleSheet);

            // 连接按钮点击信号，使用lambda表达式传递颜色
            connect(colorButton, &QPushButton::clicked, this, [=]() {
                qDebug() << "选择了颜色: " << colors[row][col].name();
                // 可以在这里发出信号，通知主窗口改变画笔颜色
                penMenu->close(); // 点击颜色后关闭菜单
            });

            gridLayout->addWidget(colorButton, row, col);

        }
    }
    colorGridAction->setDefaultWidget(colorWidget);
    penMenu->addAction(colorGridAction);
    penMenu->addSeparator();
    QAction* otherColorAction  = penMenu->addAction("其他线条颜色");
    connect(otherColorAction,&QAction::triggered,this,[=](){
        const QColor &color = QColorDialog::getColor(Qt::white,m_parentWindow,"选择颜色");
        if(color.isValid()){
            qDebug()<<"通过对话框选择了颜色："<<color.name();
            emit penColorChanged(color);
        }
    });

    QAction* pickColorAction = penMenu->addAction("取色");
    QColor colors2[1][8] = {
        {Qt::white,Qt::white,Qt::white,Qt::white,Qt::white,Qt::white,Qt::white,Qt::white,}
    };

    // 动态创建颜色按钮，并添加到网格布局中
    for (int row = 0; row < 1; ++row) {
        for (int col = 0; col < 8; ++col) {
            QPushButton *colorButton = new QPushButton();
            // 设置按钮的固定大小，使其成为一个正方形
            colorButton->setFixedSize(15, 15);
            // 设置按钮背景颜色，并添加边框
            QString styleSheet = QString("background-color: %1; border: 1px solid gray;").arg(colors2[row][col].name());
            colorButton->setStyleSheet(styleSheet);

            // 连接按钮点击信号，使用lambda表达式传递颜色
            connect(colorButton, &QPushButton::clicked, this, [=]() {
                qDebug() << "选择了颜色: " << colors2[row][col].name();
                // 可以在这里发出信号，通知主窗口改变画笔颜色
                penMenu->close(); // 点击颜色后关闭菜单
            });

            customLayout ->addWidget(colorButton, row, col);
            m_customColorButtons.append(colorButton); // 将按钮指针存起来，方便后续更新颜色

        }
    }
    colorGridAction2->setDefaultWidget(customColorWidget);
    penMenu->addAction(colorGridAction2);

    connect(pickColorAction,&QAction::triggered,this,&GraphToolBarBuilder::onPickColorTriggered );
    QAction* penAction = new QAction("画笔",m_parentWindow);
    penAction->setMenu(penMenu);
    m_toolBar->addAction(penAction);
}


void GraphToolBarBuilder::onTextTriggered()
{
    qDebug()<<"onTextTriggered";
    emit textToolTriggered();
}
void GraphToolBarBuilder::onLineTriggered()
{
    qDebug()<<"onTextTriggered";
    emit lineToolTriggered();
}
void GraphToolBarBuilder::onPolylineTriggered()
{
    qDebug()<<"onPolylineTriggered";
    emit polylineToolTriggered();
}

void GraphToolBarBuilder::onRectangleTriggered()
{
    qDebug()<<"onRectangleTriggered";
    emit rectangleToolTriggered();
}

void GraphToolBarBuilder::onEclipseTriggered()
{
    qDebug()<<"onEclipseTriggered";
    emit eclipseToolTriggered();
}

void GraphToolBarBuilder::onArcTriggered()
{
    qDebug()<<"onArcTriggered";
    emit arcToolTriggered();
}

void GraphToolBarBuilder::onPolygonTriggered()
{
    qDebug()<<"onPolygonTriggered";
    emit polygonToolTriggered();
}

void GraphToolBarBuilder::onPickColorTriggered()
{
    m_isPickingColor =true;
    QApplication::setOverrideCursor(Qt::CrossCursor);
    qApp->installEventFilter(this);

}
