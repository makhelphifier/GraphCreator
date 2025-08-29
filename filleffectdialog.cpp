#include "filleffectdialog.h"
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QPixmap>

FillEffectDialog::FillEffectDialog(QWidget *parent)
    : QDialog(parent)
{
    setupUi();
    setWindowTitle("填充效果"); // 设置窗口标题
}

FillEffectDialog::~FillEffectDialog()
{
}

void FillEffectDialog::setupUi()
{
    // --- 左侧区域 ---
    m_imagePreviewLabel = new QLabel;
    m_imagePreviewLabel->setMinimumSize(400, 300); // 设置一个最小尺寸
    m_imagePreviewLabel->setStyleSheet("border: 1px solid gray;"); // 设置边框
    m_imagePreviewLabel->setAlignment(Qt::AlignCenter);
    m_imagePreviewLabel->setText("请选择图片");

    m_selectImageButton = new QPushButton("选择图片(L)...");
    m_drawModeComboBox = new QComboBox;
    m_drawModeComboBox->addItem("居中等比拉伸");
    m_drawModeComboBox->addItem("无拉伸");
    m_drawModeComboBox->addItem("拉伸以填满");


    // 左下角布局
    QHBoxLayout *bottomLeftLayout = new QHBoxLayout;
    bottomLeftLayout->addWidget(m_selectImageButton);
    bottomLeftLayout->addStretch();

    // 整体左侧布局
    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->addWidget(new QLabel("图片:"));
    leftLayout->addWidget(m_imagePreviewLabel, 1); // 预览框占据更多空间
    leftLayout->addLayout(bottomLeftLayout);
    leftLayout->addWidget(new QLabel("绘制模式:"));
    leftLayout->addWidget(m_drawModeComboBox);


    // --- 右侧区域 ---
    m_okButton = new QPushButton("确定");
    m_cancelButton = new QPushButton("取消");
    m_examplePreviewLabel = new QLabel;
    m_examplePreviewLabel->setFixedSize(120, 90); // 固定大小的示例框
    m_examplePreviewLabel->setStyleSheet("border: 1px solid gray;");
    m_examplePreviewLabel->setAlignment(Qt::AlignCenter);


    // 右侧按钮和示例布局
    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->addWidget(m_okButton);
    rightLayout->addWidget(m_cancelButton);
    rightLayout->addStretch();
    rightLayout->addWidget(new QLabel("示例:"));
    rightLayout->addWidget(m_examplePreviewLabel);


    // --- 整体布局 ---
    QGridLayout *mainLayout = new QGridLayout(this);
    mainLayout->addLayout(leftLayout, 0, 0);
    mainLayout->addLayout(rightLayout, 0, 1);

    // --- 信号和槽连接 ---
    connect(m_selectImageButton, &QPushButton::clicked, this, &FillEffectDialog::onSelectImageClicked);
    connect(m_okButton, &QPushButton::clicked, this, &QDialog::accept); // 连接到accept()槽
    connect(m_cancelButton, &QPushButton::clicked, this, &QDialog::reject); // 连接到reject()槽
}


void FillEffectDialog::onSelectImageClicked()
{
    // 打开文件选择对话框
    QString filePath = QFileDialog::getOpenFileName(this, "选择一张图片", "", "图片文件 (*.png *.jpg *.bmp)");

    if (!filePath.isEmpty()) {
        m_selectedImagePath = filePath;
        QPixmap pixmap(filePath);

        // 更新大的预览图 (按比例缩放以适应标签大小)
        m_imagePreviewLabel->setPixmap(pixmap.scaled(m_imagePreviewLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

        // 更新右下角的示例图
        m_examplePreviewLabel->setPixmap(pixmap.scaled(m_examplePreviewLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}

QString FillEffectDialog::getSelectedImagePath() const
{
    return m_selectedImagePath;
}

// 注意：这里的返回值顺序与addItem的顺序对应
int FillEffectDialog::getDrawMode() const
{
    return m_drawModeComboBox->currentIndex();
}
