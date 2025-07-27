#ifndef PAGESETUPDIALOG_H
#define PAGESETUPDIALOG_H

#include <QWidget>
#include <QTabWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QSpinBox>
#include <QRadioButton>
#include <QDialog>


class PageSetupDialog : public QDialog
{
    Q_OBJECT
public:
    explicit PageSetupDialog(QWidget *parent = nullptr);


private:
    QTabWidget* m_tabWidget;

    QGroupBox* sizeBox;
    QGroupBox* lenBox;
    QRadioButton* customRadioButtonSize;
    QSpinBox*  widthSpinBox;
    QSpinBox*  heightSpinBox;
    QSpinBox*  leftSpinBox;
    QSpinBox*  rightSpinBox;
    QSpinBox*  upSpinBox;
    QSpinBox*  downSpinBox;
    QLabel* widthLabel;
    QLabel* heightLabel;
    QLabel* leftLabel;
    QLabel* rightLabel;
    QLabel* upLabel;
    QLabel* downLabel;

private slots:

    void on_customRadioButtonSize_clicked(bool checked = false);
signals:
};

#endif // PAGESETUPDIALOG_H
