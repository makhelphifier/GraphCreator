#ifndef GRAPHMENUBUILDER_H
#define GRAPHMENUBUILDER_H
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QMainWindow>
#include "newgraphwidget.h"
#include <QGraphicsScene>
#include "pagesetupdialog.h"
#include "previewconfigurationdialog.h"
#include "mainwindow.h"

class GraphMenuBuilder :public QObject {
    Q_OBJECT
public:
    GraphMenuBuilder();

    GraphMenuBuilder(QMainWindow* parent = nullptr);

    QMenuBar* buildMenuBar();
    
signals:
    void openGraph(const QString &filePath);
public slots:
    void onNewGraphTriggered(bool checked = false);
    void onOpenGraphTriggered(bool checked);
    void onImportGraphTriggered(bool checked);
    void onManagerGraphTriggered(bool checked);
    void onExportGraphTriggered(bool checked);
    void onPrintGraphTriggered(bool checked);
    void printGraph();
    void onPageSetupGraphTriggered();
    void on_actionUndo_triggered();
    void on_actionRedo_triggered();
    void on_actionCut_triggered();
    void on_actionCopy_triggered();
    void on_actionDelete_triggered();
    void on_actionSelectAll_triggered();
    void on_actionFindReplace_triggered();
    void on_actionPaste_triggered();
    void on_actionConfigurePreview_triggered();
    void on_actionNavigateGraph_triggered();
    void on_actionProperties_triggered();
    void on_actionStandardToolbar_triggered();
    void on_actionGraphEditToolbar_triggered();
    void on_actionAlignmentToolbar_triggered();
    void on_actionFontToolbar_triggered();
    void on_actionLineToolbar_triggered();
    void on_actionLayerManagementToolbar_triggered();
    void on_actionRunToolbar_triggered();
    void on_actionStatusBar_triggered();
    void on_actionGrid_triggered();
    void on_actionDrawingExplorer_triggered();
    void on_actionTileHorizontally_triggered();
    void on_actionTileVertically_triggered();
    void on_actionCascade_triggered();
    void on_actionArrangeIcons_triggered();
    void on_actionAbout_triggered();
    void on_actionChangelog_triggered();
    void onSaveGraphTriggered(bool checked = false);

private:
    QMainWindow* m_parentWindow;
    QMenuBar* m_menuBar;
    QGraphicsScene* m_currentScene;
    void createMenus();
    void createGraphActions();
    NewGraphWidget* m_newGraphWidget;
    PageSetupDialog* m_pageSetupDialog;
    PreviewConfigurationDialog* m_previewConfigurationDialog;

private slots:
    void onGraphFileCreated(const QString &filePath);

};

#endif // GRAPHMENUBUILDER_H
