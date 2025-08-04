#ifndef HELPINFOABOUTDIALOG_H
#define HELPINFOABOUTDIALOG_H

#include <QDialog>
#include <QWidget>
#include <QLabel>
#include <QDialogButtonBox>

class HelpInfoAboutDialog : public QDialog
{
    Q_OBJECT
public:
    HelpInfoAboutDialog();

private:
    QLabel* m_label;
    QDialogButtonBox* m_buttonBox;

};

#endif // HELPINFOABOUTDIALOG_H
