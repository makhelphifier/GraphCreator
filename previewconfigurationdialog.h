#ifndef PREVIEWCONFIGURATIONDIALOG_H
#define PREVIEWCONFIGURATIONDIALOG_H

#include <QDialog>
#include <QWidget>
#include <QGroupBox>
#include <QLineEdit>
#include <QCheckBox>
#include <QDialogButtonBox>
class PreviewConfigurationDialog : public QDialog
{
    Q_OBJECT
public:
    PreviewConfigurationDialog(QWidget* parent = nullptr);

private:
    QCheckBox* m_checkBox;
    QLineEdit* m_lineEdit;
    QGroupBox* m_groupBox;
    QDialogButtonBox* m_buttonBox;
};

#endif // PREVIEWCONFIGURATIONDIALOG_H
