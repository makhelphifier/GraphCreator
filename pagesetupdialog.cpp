#include "pagesetupdialog.h"


#include <QHBoxLayout>
PageSetupDialog::PageSetupDialog(QWidget *parent)
    : QDialog{parent}
{
    customRadioButtonSize = new QRadioButton("自定义大小(像素)",this);

    setWindowTitle("设置页面尺寸");
    resize(300,200);

    widthSpinBox = new QSpinBox(this);
    heightSpinBox= new QSpinBox(this);
    leftSpinBox= new QSpinBox(this);
    rightSpinBox= new QSpinBox(this);
    upSpinBox= new QSpinBox(this);
    downSpinBox= new QSpinBox(this);

    widthSpinBox->setRange(0,10000);
    heightSpinBox->setRange(0,10000);
    leftSpinBox->setRange(0,10000);
    rightSpinBox->setRange(0,10000);
    upSpinBox->setRange(0,10000);
    downSpinBox->setRange(0,10000);

    widthSpinBox->setValue(1584);
    heightSpinBox->setValue(768);
    leftSpinBox->setValue(10);
    rightSpinBox->setValue(10);
    upSpinBox->setValue(10);
    downSpinBox->setValue(10);

    widthLabel = new QLabel("宽度：",this);
    heightLabel = new QLabel("高度：",this);
    leftLabel = new QLabel("左：",this);
    rightLabel = new QLabel("右：",this);
    upLabel = new QLabel("上：",this);
    downLabel = new QLabel("下：",this);

    sizeBox = new QGroupBox("页面尺寸",this);
    lenBox = new QGroupBox("页边距（毫米）",this);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    QVBoxLayout* sizeBoxLayout = new QVBoxLayout(sizeBox);
    QHBoxLayout* widthLayout = new QHBoxLayout();
    QHBoxLayout* heightLayout = new QHBoxLayout();

    QVBoxLayout* lenBoxLayout = new QVBoxLayout(lenBox);
    QHBoxLayout* leftLayout = new QHBoxLayout();
    QHBoxLayout* upLayout = new QHBoxLayout();

    widthLayout->addWidget(widthLabel);
    widthLayout->addWidget(widthSpinBox);
    widthLayout->addStretch();

    heightLayout->addWidget(heightLabel);
    heightLayout->addWidget(heightSpinBox);
    heightLayout->addStretch();

    leftLayout->addWidget(leftLabel);
    leftLayout->addWidget(leftSpinBox);
    leftLayout->addStretch();
    leftLayout->addWidget(rightLabel);
    leftLayout->addWidget(rightSpinBox);

    upLayout->addWidget(upLabel);
    upLayout->addWidget(upSpinBox);
    upLayout->addStretch();
    upLayout->addWidget(downLabel);
    upLayout->addWidget(downSpinBox);

    sizeBoxLayout->addWidget(customRadioButtonSize);
    sizeBoxLayout->addLayout(widthLayout);
    sizeBoxLayout->addLayout(heightLayout);

    lenBoxLayout->addLayout(leftLayout);
    lenBoxLayout->addLayout(upLayout);

    sizeBox->setLayout(sizeBoxLayout);
    lenBox->setLayout(lenBoxLayout);

    mainLayout->addWidget(sizeBox);
    mainLayout->addWidget(lenBox);
    setLayout(mainLayout);
    on_customRadioButtonSize_clicked(false);

    connect(customRadioButtonSize,&QRadioButton::clicked,this,&PageSetupDialog::on_customRadioButtonSize_clicked);

}

void PageSetupDialog::on_customRadioButtonSize_clicked(bool checked)
{
    widthSpinBox->setEnabled(checked);
    heightSpinBox->setEnabled(checked);

}
