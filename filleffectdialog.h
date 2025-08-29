#ifndef FILLEFFECTDIALOG_H
#define FILLEFFECTDIALOG_H

#include <QDialog>
#include <QString>

// 前置声明，避免不必要的头文件包含
class QLabel;
class QPushButton;
class QComboBox;

class FillEffectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FillEffectDialog(QWidget *parent = nullptr);
    ~FillEffectDialog();

    // 公共接口，用于获取用户选择的结果
    QString getSelectedImagePath() const;
    int getDrawMode() const; // 0: 无拉伸, 1: 拉伸以填满, 2: 居中等比拉伸

private slots:
    // "选择图片"按钮的槽函数
    void onSelectImageClicked();

private:
    void setupUi(); // 初始化UI界面

    QLabel *m_imagePreviewLabel;      // 大的图片预览框
    QLabel *m_examplePreviewLabel;    // 右下角的示例预览框
    QPushButton *m_selectImageButton; // 选择图片按钮
    QComboBox *m_drawModeComboBox;    // 绘制模式下拉框
    QPushButton *m_okButton;          // 确定按钮
    QPushButton *m_cancelButton;      // 取消按钮

    QString m_selectedImagePath;      // 保存用户选择的图片路径
};

#endif // FILLEFFECTDIALOG_H
