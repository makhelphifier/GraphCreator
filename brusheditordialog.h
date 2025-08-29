#ifndef BRUSHEDITORDIALOG_H
#define BRUSHEDITORDIALOG_H

#include <QDialog>
#include <QBrush>
#include <QButtonGroup>

// 向前声明，避免在头文件中包含过多头文件
class QRadioButton;
class QStackedWidget;
class QDialogButtonBox;
class QWidget;
class QSpinBox;
class QSlider;
class QGroupBox;
class QLabel;

class BrushEditorDialog : public QDialog
{
    Q_OBJECT

public:
    // 构造函数，传入父窗口
    explicit BrushEditorDialog(QWidget *parent = nullptr);
    // 公共接口，用于在对话框关闭后获取最终配置好的画刷
    QBrush getBrush() const;

private slots:
    // 当UI控件变化时，调用此槽函数来更新画刷和预览
    void updatePreview();
    // 当单选按钮被点击时，切换堆叠窗口的页面
    void onModeChanged();
    void onColor1Changed();
    void onColor2Changed();
    void updateGradientBoxSelection(int angle);

private:
    // 私有函数，用于初始化和布局所有UI控件
    void setupUi();
    // 用于创建UI组件 ---
    QGroupBox* createColor1Group(); // 创建“颜色1”区域的函数
    QGroupBox* createColor2Group(); // 创建“颜色2”区域的函数
    QGroupBox* createGradientGroup(); // 创建“渐变”区域的函数
    QGroupBox* createRadialGroup();// 创建“放射”区域的函数
    QGroupBox* createPreviewGroup(); // 创建最终预览区域的函数
    void onRadialPresetClicked(int id); // 响应放射渐变预设按钮的点击
    void onRadialControlsChanged();     // 响应放射渐变参数的手动修改
    void updateGradientButtonStyles(); // 更新8个渐变方向按钮的样式
    void updateRadialPresetButtonStyles(); // 更新3个放射渐变预设按钮的样式

    // 存储当前正在编辑的画刷对象
    QBrush m_brush;

    // --- UI 控件成员变量 ---
    QRadioButton *m_solidRadio;         // “单色”单选按钮
    QRadioButton *m_linearGradientRadio;// “渐变”单选按钮
    QRadioButton *m_radialGradientRadio;// “放射”单选按钮

    QStackedWidget *m_stackedWidget;    // 用于切换不同模式设置页面的堆叠窗口

    QWidget *m_solidPage;               // “单色”设置页面
    QWidget *m_linearGradientPage;      // “渐变”设置页面
    QWidget *m_radialGradientPage;      // “放射”设置页面

    QWidget *m_previewWidget;           // 预览区域
    QDialogButtonBox *m_buttonBox;      // “确定”和“取消”按钮盒

    // --- 颜色1 的控件 ---
    QSpinBox *m_color1RSpinBox;      // R值输入框
    QSpinBox *m_color1GSpinBox;      // G值输入框
    QSpinBox *m_color1BSpinBox;      // B值输入框
    QSpinBox *m_color1ASpinBox;      // A值输入框
    QWidget *m_color1Preview;        // 颜色预览块
    QSlider *m_color1ASlider;        // A值滑动条

    // --- 颜色2 的控件 ---
    QSpinBox *m_color2RSpinBox;
    QSpinBox *m_color2GSpinBox;
    QSpinBox *m_color2BSpinBox;
    QSpinBox *m_color2ASpinBox;
    QWidget *m_color2Preview;
    QSlider *m_color2ASlider;

    // 渐变区域的控件 ---
    QSpinBox *m_angleSpinBox;       // 角度输入框
    QSlider *m_angleSlider;         // 角度滑动条
    QButtonGroup *m_gradientButtonGroup;

    // 放射区域的控件 ---
    // 焦点
    QSpinBox *m_focalXSpinBox;
    QSlider  *m_focalXSlider;
    QSpinBox *m_focalYSpinBox;
    QSlider  *m_focalYSlider;
    // 中心点
    QSpinBox *m_centerXSpinBox;
    QSlider  *m_centerXSlider;
    QSpinBox *m_centerYSpinBox;
    QSlider  *m_centerYSlider;
    QButtonGroup *m_radialPresetGroup; // 用于管理放射渐变的三个预设按钮
    QLabel* m_previewButton;

    QGroupBox *m_color1Group;
    QGroupBox *m_color2Group;
    QGroupBox *m_gradientGroup;
    QGroupBox *m_radialGroup;

};

#endif // BRUSHEDITORDIALOG_H
