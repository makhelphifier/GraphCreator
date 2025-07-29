#include "previewconfigurationdialog.h"
#include <QVBoxLayout>
#include <QPushButton>

PreviewConfigurationDialog::PreviewConfigurationDialog(QWidget* parent) {


    resize(500,200);
    setWindowTitle("配置测点数据访问方式");

    m_checkBox = new QCheckBox("测点数据访问路径",this);
    m_lineEdit = new QLineEdit("http://127.0.0.1:8080/ecp/",this);
    m_groupBox = new QGroupBox(this);

    m_buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel,this);
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    QVBoxLayout* groupBoxLayout = new QVBoxLayout();
    groupBoxLayout->addWidget(m_checkBox);
    groupBoxLayout->addWidget(m_lineEdit);
    m_groupBox->setLayout(groupBoxLayout);

    m_lineEdit->setEnabled(false);

    mainLayout->addWidget(m_groupBox);
    mainLayout->addWidget(m_buttonBox);
    setLayout(mainLayout);

    if(QPushButton* okButton = m_buttonBox->button(QDialogButtonBox::Ok)){
        okButton->setText("确定");
    }
    if(QPushButton* cancelButton = m_buttonBox->button(QDialogButtonBox::Cancel)){
        cancelButton->setText("取消");
    }
    connect(m_buttonBox,&QDialogButtonBox::accepted,this,&QDialog::accept);
    connect(m_buttonBox,&QDialogButtonBox::rejected,this,&QDialog::reject);
    connect(m_checkBox,&QCheckBox::clicked,this,[this](bool checked){m_lineEdit->setEnabled(checked);});

}
