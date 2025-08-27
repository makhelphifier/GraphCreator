#include "brusheditordialog.h"
#include <QtWidgets> // 包含所有常用控件的头文件
#include <QHash>
#include <QColor>


BrushEditorDialog::BrushEditorDialog(QWidget *parent)
    : QDialog(parent)
{
    // 调用私有函数来完成UI的创建和布局
    setupUi();
    // 初始化时，默认选中“单色”模式
    m_solidRadio->setChecked(true);
}

QBrush BrushEditorDialog::getBrush() const
{
    return m_brush;
}

void BrushEditorDialog::setupUi()
{
    // --- 整体布局和顶层控件 ---
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    setWindowTitle("画刷编辑器");

    // --- 1. 创建顶部的模式选择 ---
    m_solidRadio = new QRadioButton("单色");
    m_linearGradientRadio = new QRadioButton("渐变");
    m_radialGradientRadio = new QRadioButton("放射");

    QHBoxLayout *radioLayout = new QHBoxLayout;
    radioLayout->addWidget(m_solidRadio);
    radioLayout->addWidget(m_linearGradientRadio);
    radioLayout->addWidget(m_radialGradientRadio);
    QGroupBox *modeGroup = new QGroupBox;
    modeGroup->setLayout(radioLayout);
    mainLayout->addWidget(modeGroup);

    // --- 2. 创建颜色区域 ---
    // 创建一个水平布局，用于并排放置 颜色1 和 颜色2
    QHBoxLayout *colorsLayout = new QHBoxLayout;


    // 调用函数创建“颜色1”区域并添加到布局中
    colorsLayout->addWidget(createColor1Group());

    // 调用函数创建“颜色2”区域并添加到布局中
    colorsLayout->addWidget(createColor2Group());
    // 将包含了两个颜色区域的水平布局添加到主布局
    mainLayout->addLayout(colorsLayout);

    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout->addWidget(createGradientGroup());
    bottomLayout->addWidget(createRadialGroup());
    mainLayout->addLayout(bottomLayout);

    //  结果色预览区域 ---
    mainLayout->addWidget(createPreviewGroup());

    // --- 5. OK/Cancel 按钮 ---
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    mainLayout->addWidget(buttonBox);


    connect(m_solidRadio, &QRadioButton::toggled, this, &BrushEditorDialog::onModeChanged);
    connect(m_linearGradientRadio, &QRadioButton::toggled, this, &BrushEditorDialog::onModeChanged);
    connect(m_radialGradientRadio, &QRadioButton::toggled, this, &BrushEditorDialog::onModeChanged);


    // 在所有控件创建完毕后，在这里统一设置初始值
    m_color1RSpinBox->setValue(0);
    m_color1GSpinBox->setValue(0);
    m_color1BSpinBox->setValue(0);
    m_color1ASpinBox->setValue(255); // 这会触发 onColor1Changed -> updateGradientButtonStyles

    m_color2RSpinBox->setValue(255);
    m_color2GSpinBox->setValue(255);
    m_color2BSpinBox->setValue(255);
    m_color2ASpinBox->setValue(255); // 这会触发 onColor2Changed -> updateGradientButtonStyles

    m_angleSpinBox->setValue(0); // 初始化线性渐变的角度
    qobject_cast<QPushButton*>(m_radialPresetGroup->button(1))->click(); // 初始化放射渐变的预设


    // 弹簧移到最后，保证按钮总是贴着底边
    // mainLayout->addStretch(); // 如果想让预览区和按钮之间有空白，可以保留它
}


// 创建“结果色”预览区域的函数
QGroupBox* BrushEditorDialog::createPreviewGroup()
{
    QGroupBox *previewGroup = new QGroupBox("结果色");

    // GroupBox也需要一个布局来容纳它的子控件
    QVBoxLayout *layout = new QVBoxLayout;

    m_previewButton = new QLabel(this);
    m_previewButton->setFixedSize(378, 123);
    m_previewButton->setFrameStyle(QFrame::Sunken | QFrame::Panel);


    // 将预览控件添加到布局中
    layout->addWidget(m_previewButton);
    // 将布局设置给GroupBox
    previewGroup->setLayout(layout);

    return previewGroup;
}


//  创建“放射”区域的函数
QGroupBox* BrushEditorDialog::createRadialGroup()
{
    QGroupBox *radialGroup = new QGroupBox("放射");
    QVBoxLayout *mainVLayout = new QVBoxLayout;

    // --- 1. 创建顶部的 1x3 预设按钮 ---
    m_radialPresetGroup = new QButtonGroup(this);
    m_radialPresetGroup->setExclusive(true); // 设置为互斥，同时只有一个能被选中

    QHBoxLayout *previewLayout = new QHBoxLayout;
    QSize previewSize(55, 28);
    const QStringList presetLabels = {"中心", "水平", "垂直"};

    for (int i = 0; i < 3; ++i) {
        QPushButton *presetButton = new QPushButton();
        presetButton->setFixedSize(previewSize);
        presetButton->setCheckable(true); // 让按钮可以被选中
        previewLayout->addWidget(presetButton);
        m_radialPresetGroup->addButton(presetButton, i + 1); // 添加到按钮组，ID分别为1, 2, 3
    }
    mainVLayout->addLayout(previewLayout);

    // --- 2. 创建下面的控件区 ---
    // 使用 QGridLayout 可以更好地对齐标签、输入框和滑动条
    QGridLayout *controlsLayout = new QGridLayout;
    controlsLayout->setHorizontalSpacing(10);

    // 辅助函数，用于创建和连接 SpinBox 和 Slider
    auto setupControlRow = [&](QSpinBox* &spinbox, QSlider* &slider, int initialValue) {
        spinbox = new QSpinBox;
        slider = new QSlider(Qt::Horizontal);
        spinbox->setRange(0, 100);
        slider->setRange(0, 100);
        spinbox->setValue(initialValue);
        connect(slider, &QSlider::valueChanged, spinbox, &QSpinBox::setValue);
        connect(spinbox, QOverload<int>::of(&QSpinBox::valueChanged), slider, &QSlider::setValue);
    };

    // --- 渐变过渡的焦点 ---
    controlsLayout->addWidget(new QLabel("渐变过渡的焦点:"), 0, 0, 1, 3); // 标题占3列
    setupControlRow(m_focalXSpinBox, m_focalXSlider, 0);
    controlsLayout->addWidget(new QLabel("X(%):"), 1, 0);
    controlsLayout->addWidget(m_focalXSpinBox, 1, 1);
    controlsLayout->addWidget(m_focalXSlider, 1, 2);

    setupControlRow(m_focalYSpinBox, m_focalYSlider, 0);
    controlsLayout->addWidget(new QLabel("Y(%):"), 2, 0);
    controlsLayout->addWidget(m_focalYSpinBox, 2, 1);
    controlsLayout->addWidget(m_focalYSlider, 2, 2);

    // --- 中心的百分比 ---
    controlsLayout->addWidget(new QLabel("中心点的百分比:"), 3, 0, 1, 3); // 标题占3列
    setupControlRow(m_centerXSpinBox, m_centerXSlider, 50);
    controlsLayout->addWidget(new QLabel("X(%):"), 4, 0);
    controlsLayout->addWidget(m_centerXSpinBox, 4, 1);
    controlsLayout->addWidget(m_centerXSlider, 4, 2);

    setupControlRow(m_centerYSpinBox, m_centerYSlider, 50);
    controlsLayout->addWidget(new QLabel("Y(%):"), 5, 0);
    controlsLayout->addWidget(m_centerYSpinBox, 5, 1);
    controlsLayout->addWidget(m_centerYSlider, 5, 2);

    // 设置第二列（SpinBox）的宽度固定，防止被拉伸
    controlsLayout->setColumnMinimumWidth(1, 50);
    // 设置第三列（Slider）可以拉伸
    controlsLayout->setColumnStretch(2, 1);

    mainVLayout->addLayout(controlsLayout);
    radialGroup->setLayout(mainVLayout);

    // a) 点击预设按钮，调用 onRadialPresetClicked
    connect(m_radialPresetGroup, QOverload<int>::of(&QButtonGroup::idClicked), this, &BrushEditorDialog::onRadialPresetClicked);

    // b) 手动修改任何一个SpinBox的值，调用 onRadialControlsChanged
    connect(m_focalXSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &BrushEditorDialog::onRadialControlsChanged);
    connect(m_focalYSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &BrushEditorDialog::onRadialControlsChanged);
    connect(m_centerXSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &BrushEditorDialog::onRadialControlsChanged);
    connect(m_centerYSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &BrushEditorDialog::onRadialControlsChanged);

    updateRadialPresetButtonStyles();

    return radialGroup;
}


//  放射渐变预设按钮被点击的槽函数
void BrushEditorDialog::onRadialPresetClicked(int id)
{
    // 无论点击哪个按钮，先恢复所有控件的可用状态
    m_focalXSpinBox->setEnabled(true);
    m_focalXSlider->setEnabled(true);
    m_focalYSpinBox->setEnabled(true);
    m_focalYSlider->setEnabled(true);
    m_centerXSpinBox->setEnabled(true);
    m_centerXSlider->setEnabled(true);
    m_centerYSpinBox->setEnabled(true);
    m_centerYSlider->setEnabled(true);

    switch (id) {
    case 1: // 第一个按钮: 中心模式
        m_centerXSpinBox->setValue(50);
        m_centerYSpinBox->setValue(50);
        // 禁用焦点控件
        m_focalXSpinBox->setEnabled(false);
        m_focalXSlider->setEnabled(false);
        m_focalYSpinBox->setEnabled(false);
        m_focalYSlider->setEnabled(false);
        break;
    case 2: // 第二个按钮: 水平焦点
        m_focalXSpinBox->setValue(100);
        m_focalYSpinBox->setValue(0);
        // 禁用中心点控件
        m_centerXSpinBox->setEnabled(false);
        m_centerXSlider->setEnabled(false);
        m_centerYSpinBox->setEnabled(false);
        m_centerYSlider->setEnabled(false);
        break;
    case 3: // 第三个按钮: 垂直焦点
        m_focalXSpinBox->setValue(0);
        m_focalYSpinBox->setValue(100);
        // 禁用中心点控件
        m_centerXSpinBox->setEnabled(false);
        m_centerXSlider->setEnabled(false);
        m_centerYSpinBox->setEnabled(false);
        m_centerYSlider->setEnabled(false);
        break;
    }
    updatePreview(); // 更新最终预览
}

// 放射渐变参数被手动修改的槽函数
void BrushEditorDialog::onRadialControlsChanged()
{
    // 检查当前是否有预设按钮被选中
    QAbstractButton *checkedBtn = m_radialPresetGroup->checkedButton();
    if (checkedBtn) {
        // 如果有，说明用户想切换到“自定义”模式
        // 解除按钮的选中状态，并阻止其再次发射信号，避免循环调用
        m_radialPresetGroup->blockSignals(true);
        checkedBtn->setChecked(false);
        m_radialPresetGroup->blockSignals(false);

        // 同时，重新启用所有控件，因为现在是自定义模式
        // m_focalXSpinBox->setEnabled(true);
        // m_focalXSlider->setEnabled(true);
        // m_focalYSpinBox->setEnabled(true);
        // m_focalYSlider->setEnabled(true);
        // m_centerXSpinBox->setEnabled(true);
        // m_centerXSlider->setEnabled(true);
        // m_centerYSpinBox->setEnabled(true);
        // m_centerYSlider->setEnabled(true);
    }
    updatePreview(); // 更新最终预览
}

//  更新8个渐变方向按钮的背景样式
void BrushEditorDialog::updateGradientButtonStyles()
{
    // 1. 获取当前颜色1和颜色2
    QColor c1(m_color1RSpinBox->value(), m_color1GSpinBox->value(), m_color1BSpinBox->value());
    QColor c2(m_color2RSpinBox->value(), m_color2GSpinBox->value(), m_color2BSpinBox->value());

    // 2. 定义每个角度对应的渐变坐标
    // qlineargradient的坐标(x1,y1)是起点, (x2,y2)是终点, 范围是0.0到1.0
    QHash<int, QString> coordsMap;
    coordsMap[0]   = "x1:0, y1:0, x2:1, y2:0"; // 0°   (左 -> 右)
    coordsMap[45]  = "x1:0, y1:0, x2:1, y2:1"; // 45°  (左上 -> 右下)
    coordsMap[90]  = "x1:0, y1:0, x2:0, y2:1"; // 90°  (上 -> 下)
    coordsMap[135] = "x1:1, y1:0, x2:0, y2:1"; // 135° (右上 -> 左下)
    coordsMap[180] = "x1:1, y1:0, x2:0, y2:0"; // 180° (右 -> 左)
    coordsMap[225] = "x1:1, y1:1, x2:0, y2:0"; // 225° (右下 -> 左上)
    coordsMap[270] = "x1:0, y1:1, x2:0, y2:0"; // 270° (下 -> 上)
    coordsMap[315] = "x1:0, y1:1, x2:1, y2:0"; // 315° (左下 -> 右上)

    // 3. 遍历按钮组中的所有按钮，为它们设置样式
    for (QAbstractButton *button : m_gradientButtonGroup->buttons()) {
        int angle = m_gradientButtonGroup->id(button); // 获取按钮关联的角度ID

        if (coordsMap.contains(angle)) {
            // 基础样式 + 动态的渐变背景
            QString styleSheet = QString(
                                     "QPushButton {"
                                     "    background-color: qlineargradient(%1, stop:0 %2, stop:1 %3);"
                                     "    border: 1px solid gray;"
                                     "}"
                                     "QPushButton:checked, QPushButton:pressed {" // 选中或按下时，边框高亮
                                     "    border: 2px solid #0078d7;"
                                     "}"
                                     ).arg(coordsMap.value(angle)).arg(c1.name()).arg(c2.name());

            button->setStyleSheet(styleSheet);
        }
    }
}

// 更新3个放射渐变预设按钮的背景样式
void BrushEditorDialog::updateRadialPresetButtonStyles()
{
    // 1. 获取当前颜色1和颜色2
    QColor c1(m_color1RSpinBox->value(), m_color1GSpinBox->value(), m_color1BSpinBox->value());
    QColor c2(m_color2RSpinBox->value(), m_color2GSpinBox->value(), m_color2BSpinBox->value());
    QString c1Name = c1.name();
    QString c2Name = c2.name();

    // 2. 为每个按钮分别定义渐变效果
    QString gradients[] = {
        // 预设1 "中心": 保持为标准的圆形放射渐变
        QString("qradialgradient(cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5, stop:0 %1, stop:1 %2)")
            .arg(c1Name).arg(c2Name),

        // 预设2 "水平": 使用三点线性渐变模拟水平放射 (color2 -> color1 -> color2)
        QString("qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 %2, stop:0.5 %1, stop:1 %2)")
            .arg(c1Name).arg(c2Name),

        // 预设3 "垂直": 使用三点线性渐变模拟垂直放射 (color2 -> color1 -> color2)
        QString("qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 %2, stop:0.5 %1, stop:1 %2)")
            .arg(c1Name).arg(c2Name)
    };

    // 3. 循环为三个按钮应用新的样式
    for (int i = 0; i < 3; ++i) {
        if (QAbstractButton *button = m_radialPresetGroup->button(i + 1)) { // ID 是 1, 2, 3
            // 构造完整的样式表
            QString styleSheet = QString(
                                     "QPushButton {"
                                     "    background-color: %1;"
                                     "    border: 1px solid gray;"
                                     "    color: white;"
                                     "    text-shadow: 1px 1px 2px black;"
                                     "}"
                                     "QPushButton:checked, QPushButton:pressed {"
                                     "    border: 2px solid #0078d7;"
                                     "}"
                                     ).arg(gradients[i]);

            button->setStyleSheet(styleSheet);
        }
    }
}

QGroupBox* BrushEditorDialog::createGradientGroup()
{
    // 1. 创建主容器和垂直布局
    QGroupBox *gradientGroup = new QGroupBox("渐变");
    QVBoxLayout *groupLayout = new QVBoxLayout;

    // 2. 创建上半部分的 3x3 网格
    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->setSpacing(5);

    // 实例化 QButtonGroup 并设置为互斥（像单选按钮一样）
    m_gradientButtonGroup = new QButtonGroup(this);
    m_gradientButtonGroup->setExclusive(true);

    // 定义8个方向按钮的位置(col, row) -> 角度(angle)的映射
    QHash<QPoint, int> angleMap;
    angleMap[QPoint(2, 1)] = 0;   // 右
    angleMap[QPoint(2, 2)] = 45;  // 右下
    angleMap[QPoint(1, 2)] = 90;  // 下
    angleMap[QPoint(0, 2)] = 135; // 左下
    angleMap[QPoint(0, 1)] = 180; // 左
    angleMap[QPoint(0, 0)] = 225; // 左上
    angleMap[QPoint(1, 0)] = 270; // 上
    angleMap[QPoint(2, 0)] = 315; // 右上

    // 按照 3x3 布局添加8个按钮
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            if (row == 1 && col == 1) continue; // 跳过中心

            QPushButton *button = new QPushButton;
            button->setFixedSize(56, 39);
            button->setCheckable(true); // 让按钮可以被“选中”

            // 将按钮添加到布局和按钮组
            gridLayout->addWidget(button, row, col);
            int angle = angleMap.value(QPoint(col, row));
            m_gradientButtonGroup->addButton(button, angle); // 添加并设置ID为角度值
        }
    }

    // 3. 创建下半部分的滑动条和输入框
    QHBoxLayout *sliderLayout = new QHBoxLayout;
    m_angleSpinBox = new QSpinBox;
    m_angleSlider = new QSlider(Qt::Horizontal);

    // 设置范围
    m_angleSpinBox->setRange(0, 360);
    m_angleSlider->setRange(0, 360);

    // 设置SpinBox的宽度，使其不会太宽
    m_angleSpinBox->setFixedWidth(50);

    // 将控件添加到水平布局中
    sliderLayout->addWidget(m_angleSpinBox);
    sliderLayout->addWidget(m_angleSlider);

    // 4. 将网格布局和滑动条布局添加到主布局中
    groupLayout->addLayout(gridLayout);
    groupLayout->addLayout(sliderLayout);
    gradientGroup->setLayout(groupLayout);

    // 5. 联动逻辑
    // a) 滑动条和输入框互相联动 (不变)
    connect(m_angleSlider, &QSlider::valueChanged, m_angleSpinBox, &QSpinBox::setValue);
    connect(m_angleSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), m_angleSlider, &QSlider::setValue);

    // b) 点击网格按钮，更新输入框的值
    connect(m_gradientButtonGroup, QOverload<int>::of(&QButtonGroup::idClicked), m_angleSpinBox, &QSpinBox::setValue);

    // c) 输入框的值改变，更新网格按钮的选中状态
    connect(m_angleSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &BrushEditorDialog::updateGradientBoxSelection);

    // 初始化按钮的渐变背景色
    updateGradientButtonStyles();

    return gradientGroup;
}

void BrushEditorDialog::updateGradientBoxSelection(int angle)
{
    // 根据角度值查找对应的按钮
    QAbstractButton *buttonToSelect = m_gradientButtonGroup->button(angle);

    if (buttonToSelect) {
        // 如果找到了完全匹配的按钮 (0, 45, 90 ...)，则选中它
        buttonToSelect->setChecked(true);
    } else {
        // 如果角度是中间值 (例如 25°)，则取消当前所有按钮的选中状态
        QAbstractButton *checkedBtn = m_gradientButtonGroup->checkedButton();
        if (checkedBtn) {
            // 关键：临时阻断信号，防止 setChecked(false) 触发 idClicked(-1) 信号导致循环或意外行为
            m_gradientButtonGroup->blockSignals(true);
            checkedBtn->setChecked(false);
            m_gradientButtonGroup->blockSignals(false);
        }
    }
    updatePreview();
}

QGroupBox* BrushEditorDialog::createColor1Group()
{
    QGroupBox *color1Group = new QGroupBox("颜色1");
    QHBoxLayout *color1Layout = new QHBoxLayout;

    // 左侧RGBA输入
    QFormLayout *rgbaLayout = new QFormLayout;
    m_color1RSpinBox = new QSpinBox;
    m_color1GSpinBox = new QSpinBox;
    m_color1BSpinBox = new QSpinBox;
    m_color1ASpinBox = new QSpinBox;
    m_color1RSpinBox->setRange(0, 255);
    m_color1GSpinBox->setRange(0, 255);
    m_color1BSpinBox->setRange(0, 255);
    m_color1ASpinBox->setRange(0, 255);
    rgbaLayout->addRow("R:", m_color1RSpinBox);
    rgbaLayout->addRow("G:", m_color1GSpinBox);
    rgbaLayout->addRow("B:", m_color1BSpinBox);
    rgbaLayout->addRow("A:", m_color1ASpinBox);

    // 右侧区域 (色板 + 滑动条)
    QVBoxLayout *rightSideLayout = new QVBoxLayout;
    QGridLayout *colorGrid = new QGridLayout;
    colorGrid->setSpacing(2);

    m_color1Preview = new QWidget;
    m_color1Preview->setFixedSize(66, 44);
    m_color1Preview->setAutoFillBackground(true);
    QPalette p1 = m_color1Preview->palette();
    p1.setColor(QPalette::Window, Qt::gray);
    m_color1Preview->setPalette(p1);
    colorGrid->addWidget(m_color1Preview, 0, 0, 2, 3);

    QList<QColor> colors = {Qt::red, Qt::green, Qt::blue, Qt::white, Qt::black, Qt::gray, Qt::cyan, Qt::magenta, Qt::yellow};
    const QPoint gridPositions[] = {
        {3, 0}, {4, 0}, // row 0
        {3, 1}, {4, 1}, // row 1
        {0, 2}, {1, 2}, {2, 2}, {3, 2}, {4, 2} // row 2
    };

    for (int i = 0; i < colors.size(); ++i) {
        const QColor &color = colors[i];
        const QPoint &pos = gridPositions[i];

        QPushButton *swatchButton = new QPushButton;
        swatchButton->setFixedSize(20, 20);
        swatchButton->setCursor(Qt::PointingHandCursor);
        swatchButton->setStyleSheet(QString("background-color: %1; border: 1px solid gray;").arg(color.name()));
        swatchButton->setFlat(true);

        connect(swatchButton, &QPushButton::clicked, this, [this, color]() {
            m_color1RSpinBox->setValue(color.red());
            m_color1GSpinBox->setValue(color.green());
            m_color1BSpinBox->setValue(color.blue());
        });

        // addWidget(widget, row, col) -> pos.y()是行, pos.x()是列
        colorGrid->addWidget(swatchButton, pos.y(), pos.x());
    }

    m_color1ASlider = new QSlider(Qt::Horizontal);
    m_color1ASlider->setRange(0, 255);
    rightSideLayout->addLayout(colorGrid);
    rightSideLayout->addWidget(m_color1ASlider);

    // 组合
    color1Layout->addLayout(rgbaLayout);
    color1Layout->addLayout(rightSideLayout);
    color1Group->setLayout(color1Layout);

    //  信号连接 ---
    // 1. RGBA输入框的变化，统一触发 onColor1Changed
    connect(m_color1RSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &BrushEditorDialog::onColor1Changed);
    connect(m_color1GSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &BrushEditorDialog::onColor1Changed);
    connect(m_color1BSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &BrushEditorDialog::onColor1Changed);
    connect(m_color1ASpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &BrushEditorDialog::onColor1Changed);

    // 2. 透明度(A)的 SpinBox 和 Slider 互相联动
    connect(m_color1ASlider, &QSlider::valueChanged, m_color1ASpinBox, &QSpinBox::setValue);
    connect(m_color1ASpinBox, QOverload<int>::of(&QSpinBox::valueChanged), m_color1ASlider, &QSlider::setValue);

    return color1Group;
}

//  创建“颜色2”区域的函数 (基本是 color1 的复制)
QGroupBox* BrushEditorDialog::createColor2Group()
{
    QGroupBox *color2Group = new QGroupBox("颜色2"); // <-- 标题不同
    QHBoxLayout *color2Layout = new QHBoxLayout;

    // 左侧RGBA输入
    QFormLayout *rgbaLayout = new QFormLayout;
    m_color2RSpinBox = new QSpinBox; // <-- 使用 m_color2... 变量
    m_color2GSpinBox = new QSpinBox;
    m_color2BSpinBox = new QSpinBox;
    m_color2ASpinBox = new QSpinBox;
    m_color2RSpinBox->setRange(0, 255);
    m_color2GSpinBox->setRange(0, 255);
    m_color2BSpinBox->setRange(0, 255);
    m_color2ASpinBox->setRange(0, 255);
    rgbaLayout->addRow("R:", m_color2RSpinBox);
    rgbaLayout->addRow("G:", m_color2GSpinBox);
    rgbaLayout->addRow("B:", m_color2BSpinBox);
    rgbaLayout->addRow("A:", m_color2ASpinBox);

    // 右侧区域 (色板 + 滑动条)
    QVBoxLayout *rightSideLayout = new QVBoxLayout;
    QGridLayout *colorGrid = new QGridLayout;
    colorGrid->setSpacing(2);

    m_color2Preview = new QWidget; // <-- 使用 m_color2... 变量
    m_color2Preview->setFixedSize(66, 44);
    m_color2Preview->setAutoFillBackground(true);
    QPalette p2 = m_color2Preview->palette();
    p2.setColor(QPalette::Window, Qt::gray);
    m_color2Preview->setPalette(p2);
    colorGrid->addWidget(m_color2Preview, 0, 0, 2, 3);

    QList<QColor> colors = {Qt::red, Qt::green, Qt::blue, Qt::white, Qt::black, Qt::gray, Qt::cyan, Qt::magenta, Qt::yellow};
    // 修正了坐标 (col, row)
    const QPoint gridPositions[] = {
        {3, 0}, {4, 0}, // row 0
        {3, 1}, {4, 1}, // row 1
        {0, 2}, {1, 2}, {2, 2}, {3, 2}, {4, 2} // row 2
    };

    for (int i = 0; i < colors.size(); ++i) {
        const QColor &color = colors[i];
        const QPoint &pos = gridPositions[i];

        QPushButton *swatchButton = new QPushButton;
        swatchButton->setFixedSize(20, 20);
        swatchButton->setCursor(Qt::PointingHandCursor);
        swatchButton->setStyleSheet(QString("background-color: %1; border: 1px solid gray;").arg(color.name()));
        swatchButton->setFlat(true);

        connect(swatchButton, &QPushButton::clicked, this, [this, color]() {
            m_color2RSpinBox->setValue(color.red());
            m_color2GSpinBox->setValue(color.green());
            m_color2BSpinBox->setValue(color.blue());
        });

        // addWidget(widget, row, col) -> pos.y()是行, pos.x()是列
        colorGrid->addWidget(swatchButton, pos.y(), pos.x());
    }


    m_color2ASlider = new QSlider(Qt::Horizontal); // <-- 使用 m_color2... 变量
    m_color2ASlider->setRange(0, 255);
    rightSideLayout->addLayout(colorGrid);
    rightSideLayout->addWidget(m_color2ASlider);

    // 组合
    color2Layout->addLayout(rgbaLayout);
    color2Layout->addLayout(rightSideLayout);
    color2Group->setLayout(color2Layout);

    //  信号连接 ---
    // 1. RGBA输入框的变化，统一触发 onColor2Changed
    connect(m_color2RSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &BrushEditorDialog::onColor2Changed);
    connect(m_color2GSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &BrushEditorDialog::onColor2Changed);
    connect(m_color2BSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &BrushEditorDialog::onColor2Changed);
    connect(m_color2ASpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &BrushEditorDialog::onColor2Changed);

    // 2. 透明度(A)的 SpinBox 和 Slider 互相联动
    connect(m_color2ASlider, &QSlider::valueChanged, m_color2ASpinBox, &QSpinBox::setValue);
    connect(m_color2ASpinBox, QOverload<int>::of(&QSpinBox::valueChanged), m_color2ASlider, &QSlider::setValue);


    return color2Group;
}


void BrushEditorDialog::onModeChanged()
{
    // // 根据哪个单选按钮被选中，来切换QStackedWidget显示的页面
    // if (m_solidRadio->isChecked()) {
    //     m_stackedWidget->setCurrentWidget(m_solidPage);
    // } else if (m_linearGradientRadio->isChecked()) {
    //     m_stackedWidget->setCurrentWidget(m_linearGradientPage);
    // } else if (m_radialGradientRadio->isChecked()) {
    //     m_stackedWidget->setCurrentWidget(m_radialGradientPage);
    // }
    updatePreview();
}

// 处理颜色1变化的槽函数
void BrushEditorDialog::onColor1Changed()
{
    // 1. 从SpinBox获取当前的RGBA值
    int r = m_color1RSpinBox->value();
    int g = m_color1GSpinBox->value();
    int b = m_color1BSpinBox->value();
    int a = m_color1ASpinBox->value();

    // 2. 更新“颜色1”区域的小预览块颜色
    QColor color(r, g, b, a);
    QPalette p = m_color1Preview->palette();
    p.setColor(QPalette::Window, color);
    m_color1Preview->setPalette(p);

    updateGradientButtonStyles(); // 更新方向按钮的渐变色
    updateRadialPresetButtonStyles();


    // 3. 触发“结果色”大预览区的更新
    updatePreview();
}

// 处理颜色2变化的槽函数
void BrushEditorDialog::onColor2Changed()
{
    // 逻辑与 onColor1Changed 完全相同，只是操作 color2 的控件
    int r = m_color2RSpinBox->value();
    int g = m_color2GSpinBox->value();
    int b = m_color2BSpinBox->value();
    int a = m_color2ASpinBox->value();

    QColor color(r, g, b, a);
    QPalette p = m_color2Preview->palette();
    p.setColor(QPalette::Window, color);
    m_color2Preview->setPalette(p);

    updateGradientButtonStyles(); // 更新方向按钮的渐变色
    updateRadialPresetButtonStyles();


    updatePreview();
}


// 根据UI控件更新画刷和最终预览
void BrushEditorDialog::updatePreview()
{
    // 获取颜色1和颜色2，渐变模式下会用到
    QColor color1(m_color1RSpinBox->value(), m_color1GSpinBox->value(), m_color1BSpinBox->value(), m_color1ASpinBox->value());
    QColor color2(m_color2RSpinBox->value(), m_color2GSpinBox->value(), m_color2BSpinBox->value(), m_color2ASpinBox->value());

    if (m_solidRadio->isChecked()) {
        // --- 1. 单色模式 ---
        m_brush = QBrush(color1);

    } else if (m_linearGradientRadio->isChecked()) {
        // --- 2. 线性渐变模式 ---
        // 将角度转换为梯度线坐标
        double angleRad = qDegreesToRadians(static_cast<double>(m_angleSpinBox->value()));
        double x1 = 0.5 - qCos(angleRad) / 2.0;
        double y1 = 0.5 - qSin(angleRad) / 2.0;
        double x2 = 0.5 + qCos(angleRad) / 2.0;
        double y2 = 0.5 + qSin(angleRad) / 2.0;

        // 创建线性渐变对象
        QLinearGradient linearGradient(x1, y1, x2, y2);
        // 使用 ObjectBoundingMode 模式，坐标(0,0)-(1,1)会自动映射到绘制区域
        linearGradient.setCoordinateMode(QGradient::ObjectBoundingMode);
        linearGradient.setColorAt(0.0, color1); // 0.0处为颜色1
        linearGradient.setColorAt(1.0, color2); // 1.0处为颜色2

        m_brush = QBrush(linearGradient);

    } else if (m_radialGradientRadio->isChecked()) {
        // --- 3. 放射渐变模式---

        // 获取当前选中的预设按钮ID
        int checkedId = m_radialPresetGroup->checkedId();

        // 优先根据预设按钮ID来创建画刷
        if (checkedId == 1) { // 预设1: 中心放射
            QRadialGradient gradient(0.5, 0.5, 0.5, 0.5, 0.5); // 中心点、半径、焦点都在正中
            gradient.setCoordinateMode(QGradient::ObjectBoundingMode);
            gradient.setColorAt(0.0, color1);
            gradient.setColorAt(1.0, color2);
            m_brush = QBrush(gradient);

        } else if (checkedId == 2) { // 预设2: 水平放射 (用线性渐变模拟)
            QLinearGradient gradient(0.0, 0.5, 1.0, 0.5); // 从左到右
            gradient.setCoordinateMode(QGradient::ObjectBoundingMode);
            // 实现 color2 -> color1 -> color2 的效果
            gradient.setColorAt(0.0, color2);
            gradient.setColorAt(0.5, color1);
            gradient.setColorAt(1.0, color2);
            m_brush = QBrush(gradient);

        } else if (checkedId == 3) { // 预设3: 垂直放射 (用线性渐变模拟)
            QLinearGradient gradient(0.5, 0.0, 0.5, 1.0); // 从上到下
            gradient.setCoordinateMode(QGradient::ObjectBoundingMode);
            // 实现 color2 -> color1 -> color2 的效果
            gradient.setColorAt(0.0, color2);
            gradient.setColorAt(0.5, color1);
            gradient.setColorAt(1.0, color2);
            m_brush = QBrush(gradient);

        } else {
            // 其他情况(自定义模式): 用户手动调整了滑块, 此时没有任何预设按钮被选中
            // 此时我们使用原有的逻辑，从SpinBox读取精确值
            double centerX = m_centerXSpinBox->value() / 100.0;
            double centerY = m_centerYSpinBox->value() / 100.0;
            double focalX = m_focalXSpinBox->value() / 100.0;
            double focalY = m_focalYSpinBox->value() / 100.0;
            double radius = 0.5;

            QRadialGradient radialGradient(centerX, centerY, radius, focalX, focalY);
            radialGradient.setCoordinateMode(QGradient::ObjectBoundingMode);
            radialGradient.setColorAt(0.0, color1);
            radialGradient.setColorAt(1.0, color2);
            m_brush = QBrush(radialGradient);
        }
    }

    // --- 统一使用 m_brush 绘制预览 ---
    QImage img(378, 123, QImage::Format_ARGB32_Premultiplied);
    img.fill(Qt::transparent); // 先填充透明背景
    QPainter p(&img);
    p.fillRect(img.rect(), m_brush);
    p.end();

    QPixmap pix = QPixmap::fromImage(img);
    // m_previewButton->setIcon(QIcon(pix));
    // m_previewButton->/*setIconSize*/(m_previewButton->size());
    // 如果您采纳了之前的建议换成了QLabel，请使用下面这行代码
    qobject_cast<QLabel*>(m_previewButton)->setPixmap(pix);
}
