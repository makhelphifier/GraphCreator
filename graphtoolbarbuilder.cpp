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
#include "filleffectdialog.h"
#include <QPainter>
#include "pen_size_widget.h"
#include "pen_style_widget.h"
#include "penarrowwidget.h"
#include <QWidgetAction>


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
    createPenSizeMenu();
    createPenStyleMenu();
    createPenArrowMenu();
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

    QAction* otherColorAction  = fillColorMenu->addAction("其他填充颜色");

    connect(otherColorAction, &QAction::triggered, this, [=](){
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
    QAction* fillImageAction  = fillColorMenu->addAction("填充效果");
    connect(fillImageAction, &QAction::triggered, this, &GraphToolBarBuilder::onFillEffectTriggered);

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


void GraphToolBarBuilder::createPenSizeMenu()
{
    QMenu* penSizeMenu = new QMenu(m_parentWindow);
    QAction* penSizeAction = new QAction("线宽",m_parentWindow);
    penSizeAction->setMenu(penSizeMenu);

    QPixmap mainIconPixmap(24,24);
    mainIconPixmap.fill(Qt::transparent);
    QPainter iconPainter(&mainIconPixmap);
    QPen pen(Qt::black);
    pen.setCapStyle(Qt::FlatCap);
    pen.setWidth(1);
    iconPainter.setPen(pen);
    iconPainter.drawLine(3,7,21,7);
    pen.setWidth(2);
    iconPainter.setPen(pen);
    iconPainter.drawLine(3,12,21,12);
    pen.setWidth(3);
    iconPainter.setPen(pen);
    iconPainter.drawLine(3,17,21,17);
    penSizeAction->setIcon(QIcon(mainIconPixmap));
    const QList<int> pointSizes = {1,2,3,4,5};
    for (int size : pointSizes) {
        QWidgetAction* wa = new QWidgetAction(penSizeMenu);
        PenSizeWidget* w = new PenSizeWidget(size);
        wa->setDefaultWidget(w);
        penSizeMenu->addAction(wa);

        connect(w, &PenSizeWidget::activated, [=](int s){
            qDebug() << "selected pen size :" << s << "pt";
            // 在这里把笔宽应用到你的工具/变量上
            penSizeMenu->hide(); // 选完后关闭菜单（可选）
        });
    }

    m_toolBar->addAction(penSizeAction);

}

void GraphToolBarBuilder::createPenStyleMenu()
{
    QMenu* penStyleMenu = new QMenu(m_parentWindow);
    QAction* penStyleAction = new QAction("线型",m_parentWindow);
    penStyleAction->setMenu(penStyleMenu);

    QPixmap mainIconPixmap(24,24);
    mainIconPixmap.fill(Qt::transparent);
    QPainter iconPainter(&mainIconPixmap);
    QPen pen(Qt::black);
    pen.setCapStyle(Qt::FlatCap);
    pen.setStyle(Qt::SolidLine);
    iconPainter.setPen(pen);
    iconPainter.drawLine(3,7,21,7);
    pen.setStyle(Qt::DotLine);
    iconPainter.setPen(pen);
    iconPainter.drawLine(3,12,21,12);
    pen.setStyle(Qt::DashDotLine);
    iconPainter.setPen(pen);
    iconPainter.drawLine(3,17,21,17);
    penStyleAction->setIcon(QIcon(mainIconPixmap));
    // 菜单里添加各种线型
    QList<Qt::PenStyle> styles = {
        Qt::SolidLine,
        Qt::DashLine,
        Qt::DotLine,
        Qt::DashDotLine,
        Qt::DashDotDotLine
    };

    for (Qt::PenStyle style : styles) {
        QWidgetAction* wa = new QWidgetAction(penStyleMenu);
        PenStyleWidget* w = new PenStyleWidget(style);
        wa->setDefaultWidget(w);
        penStyleMenu->addAction(wa);

        connect(w, &PenStyleWidget::activated, [=](Qt::PenStyle s){
            qDebug() << "selected pen style :" << s;
            // TODO: 应用到画笔
            penStyleMenu->hide();
        });
    }

    m_toolBar->addAction(penStyleAction);

}

// 确保 M_PI 定义存在
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
void GraphToolBarBuilder::createPenArrowMenu()
{
    QMenu* penArrowMenu = new QMenu(m_parentWindow);
    QAction* penArrowAction = new QAction("箭头样式", m_parentWindow);
    penArrowAction->setMenu(penArrowMenu);

    // 为工具栏按钮创建一个图标
    QPixmap mainIconPixmap(24, 24);
    mainIconPixmap.fill(Qt::transparent);
    QPainter iconPainter(&mainIconPixmap);
    iconPainter.setRenderHint(QPainter::Antialiasing);
    QPen iconPen(Qt::black, 2);
    iconPen.setCapStyle(Qt::FlatCap);
    iconPainter.setPen(iconPen);
    iconPainter.setBrush(Qt::black);

    // --- 错误修正部分 ---
    // 直接在这里绘制图标，不再调用外部的 drawArrow 函数
    QPointF start(4, 12);
    QPointF end(20, 12);
    iconPainter.drawLine(start, end); // 绘制中间的线

    double arrowSize = 6.0;
    // 绘制右箭头
    double angleRight = std::atan2(end.y() - start.y(), end.x() - start.x());
    QPointF p1_right = end - QPointF(arrowSize * std::cos(angleRight - M_PI / 6), arrowSize * std::sin(angleRight - M_PI / 6));
    QPointF p2_right = end - QPointF(arrowSize * std::cos(angleRight + M_PI / 6), arrowSize * std::sin(angleRight + M_PI / 6));
    iconPainter.drawPolygon(QPolygonF() << end << p1_right << p2_right);

    // 绘制左箭头
    double angleLeft = std::atan2(start.y() - end.y(), start.x() - end.x());
    QPointF p1_left = start - QPointF(arrowSize * std::cos(angleLeft - M_PI / 6), arrowSize * std::sin(angleLeft - M_PI / 6));
    QPointF p2_left = start - QPointF(arrowSize * std::cos(angleLeft + M_PI / 6), arrowSize * std::sin(angleLeft + M_PI / 6));
    iconPainter.drawPolygon(QPolygonF() << start << p1_left << p2_left);
    // --- 修正结束 ---

    penArrowAction->setIcon(QIcon(mainIconPixmap));

    // 定义所有需要创建的样式
    QList<ArrowStyle> styles = {
        ArrowStyle::NoArrow,
        ArrowStyle::RightArrow,
        ArrowStyle::LeftArrow,
        ArrowStyle::BothArrowsOut,
        ArrowStyle::RightArrowIn,
        ArrowStyle::LeftArrowIn,
        ArrowStyle::BothArrowsIn
    };

    // 循环创建每个菜单项
    for (ArrowStyle style : styles) {
        QWidgetAction* wa = new QWidgetAction(penArrowMenu);
        PenArrowWidget* w = new PenArrowWidget(style);
        wa->setDefaultWidget(w);
        penArrowMenu->addAction(wa);

        // 连接自定义控件的 activated 信号
        connect(w, &PenArrowWidget::activated, [=](ArrowStyle s){
            qDebug() << "selected arrow style :" << static_cast<int>(s);
            // 发射信号，通知其他部分箭头样式已更改
            // emit penArrowStyleChanged(s);
            penArrowMenu->hide(); // 选择后关闭菜单
        });
    }

    m_toolBar->addAction(penArrowAction);
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


// 在 graphtoolbarbuilder.cpp 文件末尾添加
void GraphToolBarBuilder::onFillEffectTriggered()
{
    FillEffectDialog dialog(m_parentWindow);
    if (dialog.exec() == QDialog::Accepted) {
        QString imagePath = dialog.getSelectedImagePath();
        int drawMode = dialog.getDrawMode();

        if (imagePath.isEmpty()) {
            qDebug() << "未选择图片，操作取消。";
            return;
        }

        qDebug() << "用户点击了'确定'";
        qDebug() << "选择的图片路径是: " << imagePath;

        QString modeStr;
        switch(drawMode) {
        case 0: modeStr = "居中等比拉伸"; break;
        case 1: modeStr = "无拉伸"; break;
        case 2: modeStr = "拉伸以填满"; break;
        default: modeStr = "未知"; break;
        }
        qDebug() << "选择的绘制模式是: " << modeStr;

        // TODO: 在这里可以发射一个信号，将图片路径和绘制模式传递给主窗口
        // 例如: emit fillEffectChanged(imagePath, drawMode);
    } else {
        qDebug() << "用户点击了'取消'或关闭了对话框";
    }
}
