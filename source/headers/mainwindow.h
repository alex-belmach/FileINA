#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "pane.h"
#include "settings.h"
#include "properties.h"
#include <QMainWindow>
#include <QSplitter>
#include <QTreeView>
#include <QMenuBar>
#include <QMenu>
#include <QFileSystemModel>
#include <QSortFilterProxyModel>
#include <QModelIndex>
#include <QAction>
#include <QMessageBox>
#include <QClipboard>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:                                 //get rid of public variables
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void showContextMenu(QPoint);
    void setCurrentIndex(const QModelIndex &);
    bool viewIsFocused(QWidget* , bool);
    void toolBarSetVisible(bool);
    bool toolBarVisibility();
    QFileSystemModel* getFileSystemModel();

private:
    QTreeView *dirTreeView;
    QFileSystemModel *fileSystemModel;
    QSortFilterProxyModel *fileSystemProxyModel;
    Pane *leftPane;
    Pane *rightPane;
    Pane *activePane;
    QToolBar *toolBar;
    QSplitter *splitter;
    QMenuBar *menuBar;
    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *viewMenu;
    QMenu *helpMenu;
    QMenu *contextMenu;
    QItemSelectionModel* dirSelectionModel;
    QAction *aboutAction;
    QAction *openAction;
    QAction *newFolderAction;
    QAction *deleteAction;
    QAction *settingsAction;
    QAction *exitAction;
    QAction *copyAction;
    QAction *pasteAction;
    QAction *cutAction;
    QAction *tableViewAction;
    QAction *listViewAction;
    QAction *propertiesAction;
    QAction *showHiddenAction;
    QActionGroup *viewActionGroup;

    void createMenusAndActions();
    void setActivePane(Pane *);
    Pane* getActivePane();
    void updateViewActions();

private slots:
    void slotFocusChanged(QWidget*, QWidget*);
    void slotDirSelectionChanged(QModelIndex, QModelIndex);
    void slotShowAbout();
    void slotNewFolder();
    void slotDelete();
    void slotSettings();
    void slotCopy();
    void slotPaste();
    void slotCut();
    void slotTableView();
    void slotListView();
    void slotShowProperties();
    void slotContextMenu(const QPoint &);
    void slotShowHidden();
};

class FileSystemFilterProxyModel : public QSortFilterProxyModel
{
protected:
    virtual bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;
};

#endif // MAINWINDOW_H
