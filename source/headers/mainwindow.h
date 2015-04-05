#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "pane.h"
#include "settings.h"
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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:                                 //get rid of public variables
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    Pane *leftPane;
    Pane *rightPane;
    Pane *activePane;
    QToolBar *toolBar;
    QFileSystemModel *fileSystemModel;
    QSortFilterProxyModel *fileSystemProxyModel;
    QTreeView *dirTreeView;

private:
    QSplitter *splitter;
    QMenuBar *menuBar;
    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *viewMenu;
    QMenu *helpMenu;
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
    QActionGroup *viewActionGroup;

    void createMenusAndActions();
    void setActivePane(Pane *);
    Pane* getActivePane();
    void updateViewActions();

private slots:
    void slotFocusChanged(QWidget*, QWidget*);
    void slotDirSelectionChanged(QModelIndex, QModelIndex);
    void slotShowAbout();
    void slotOpen();
    void slotNewFolder();
    void slotDelete();
    void slotSettings();
    void slotCopy();
    void slotPaste();
    void slotCut();
    void slotTableView();
    void slotListView();
};

class FileSystemFilterProxyModel : public QSortFilterProxyModel
{
protected:
    virtual bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;
};

#endif // MAINWINDOW_H
