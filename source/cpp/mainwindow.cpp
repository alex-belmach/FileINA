#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setWindowTitle("FileINA");
    setWindowIcon(QIcon(":/Images/Application.png"));

    createMenusAndActions();

    settings = new QSettings("Livorni", "FileINA");

    splitter = new QSplitter(this);

    fileSystemModel = new QFileSystemModel;
    fileSystemModel->setFilter(QDir::NoDot | QDir::AllEntries | QDir::System);
    fileSystemModel->setRootPath("");
    fileSystemModel->setReadOnly(false);

    fileSystemProxyModel = new FileSystemFilterProxyModel();
    fileSystemProxyModel->setSourceModel(fileSystemModel);

    dirTreeView = new QTreeView(splitter);
    dirTreeView->setHeaderHidden(true);
    dirTreeView->setUniformRowHeights(true);
    dirTreeView->setModel(fileSystemProxyModel);
    dirTreeView->hideColumn(1);
    dirTreeView->hideColumn(2);
    dirTreeView->hideColumn(3);
    dirTreeView->setEditTriggers(QAbstractItemView::EditKeyPressed | QAbstractItemView::SelectedClicked);
    dirTreeView->setDragDropMode(QAbstractItemView::DropOnly);
    dirTreeView->setDefaultDropAction(Qt::MoveAction);
    dirTreeView->setDropIndicatorShown(true);

    dirSelectionModel = dirTreeView->selectionModel();
    connect(dirSelectionModel, SIGNAL(currentChanged(QModelIndex, QModelIndex)), this, SLOT(slotDirSelectionChanged(QModelIndex, QModelIndex)));

    leftPane = new Pane(splitter);
    rightPane = new Pane(splitter);
    setActivePane(leftPane);

    splitter->addWidget(dirTreeView);
    splitter->addWidget(leftPane);
    splitter->addWidget(rightPane);
    splitter->setHandleWidth(3);
    this->setCentralWidget(splitter);

    connect(QApplication::clipboard(), SIGNAL(changed(QClipboard::Mode)), this, SLOT(slotClipboardChanged()));
    connect(qApp, SIGNAL(focusChanged(QWidget*, QWidget*)), SLOT(slotFocusChanged(QWidget*, QWidget*)));

    restoreSettings();
    qApp->processEvents();
}

void MainWindow::slotContextMenu(const QPoint& pos)
{
    showContextMenu(dirTreeView->mapToGlobal(pos));
}

void MainWindow::slotFocusChanged(QWidget*, QWidget* focus)
{
    if (rightPane->isFocused(focus, true))
        setActivePane(rightPane);
    else if (leftPane->isFocused(focus, true))
        setActivePane(leftPane);
}

void MainWindow::toolBarSetVisible(bool visibility)
{
    toolBar->setVisible(visibility);
}

bool MainWindow::toolBarVisibility()
{
    return toolBar->isVisible();
}

void MainWindow::slotDirSelectionChanged(QModelIndex current, QModelIndex )
{
    QFileInfo fileInfo(fileSystemModel->fileInfo(fileSystemProxyModel->mapToSource(current)));
    if (!fileInfo.exists())
        return;
    activePane->changeTo(fileInfo.filePath());
}

Pane* MainWindow::getActivePane()
{
    return (activePane);
}

void MainWindow::setActivePane(Pane * pane)
{
    pane->setActive(true);
    if (pane == rightPane)
        leftPane->setActive(false);
    else
        rightPane->setActive(false);
    activePane = pane;
    updateViewActions();
}

void MainWindow::setCurrentIndex(const QModelIndex &index)
{
    dirTreeView->setCurrentIndex(fileSystemProxyModel->mapFromSource(index));
}

bool MainWindow::viewIsFocused(QWidget *focus, bool withPath)
{
    if ((leftPane->isFocused(focus, withPath)) | (rightPane->isFocused(focus, withPath)))
        return true;
    else
        return false;
}

void MainWindow::updateViewActions()
{
    if (activePane->getCurrentView() == Pane::Table)
        tableViewAction->setChecked(true);
    else
        listViewAction->setChecked(true);
}

void MainWindow::createMenusAndActions()
{
    menuBar = new QMenuBar(0);

    aboutAction = new QAction(QIcon::fromTheme("about", QIcon(":/Images/About.png")), "About", this);
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(slotShowAbout()));

    newFolderAction = new QAction(QIcon::fromTheme("new", QIcon(":/Images/NewFolder.png")), "New Folder", this);
    newFolderAction->setShortcut(QKeySequence::New);
    connect(newFolderAction, SIGNAL(triggered()), this, SLOT(slotNewFolder()));

    deleteAction = new QAction(QIcon::fromTheme("delete", QIcon(":/Images/Delete.png")), "Delete", this);
    deleteAction->setShortcut(QKeySequence::Delete);
    connect(deleteAction, SIGNAL(triggered()), this, SLOT(slotDelete()));

    settingsAction = new QAction(QIcon::fromTheme("settings", QIcon(":/Images/Preferences.png")), "Settings", this);
    connect(settingsAction, SIGNAL(triggered()), this, SLOT(slotSettings()));

    exitAction = new QAction(QIcon::fromTheme("exit", QIcon(":/Images/Exit.png")), "Exit", this);
    exitAction->setShortcut(QKeySequence::Quit);
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    copyAction = new QAction(QIcon::fromTheme("copy", QIcon(":/Images/Copy.png")), "Copy", this);
    copyAction->setShortcut(QKeySequence::Copy);
    connect(copyAction, SIGNAL(triggered()), this, SLOT(slotCopy()));

    pasteAction = new QAction(QIcon::fromTheme("paste", QIcon(":/Images/Paste.png")), "Paste", this);
    pasteAction->setShortcut(QKeySequence::Paste);
    connect(pasteAction, SIGNAL(triggered()), this, SLOT(slotPaste()));

    cutAction = new QAction(QIcon::fromTheme("cut", QIcon(":/Images/Cut.png")), "Cut", this);
    cutAction->setShortcut(QKeySequence::Cut);
    connect(cutAction, SIGNAL(triggered()), this, SLOT(slotCut()));

    propertiesAction = new QAction(QIcon::fromTheme("properties", QIcon(":/Images/Properties.png")), "Properties", this);
    propertiesAction->setShortcut(QKeySequence::Preferences);
    connect(propertiesAction, SIGNAL(triggered()), this, SLOT(slotShowProperties()));

    tableViewAction = new QAction(QIcon::fromTheme("tableview", QIcon(":/Images/TableView.png")), "Table View", this);
    tableViewAction->setCheckable(true);
    connect(tableViewAction, SIGNAL(triggered()), this, SLOT(slotTableView()));

    listViewAction = new QAction(QIcon::fromTheme("listview", QIcon(":/Images/ListView.png")), "List View", this);
    listViewAction->setCheckable(true);
    connect(listViewAction, SIGNAL(triggered()), this, SLOT(slotListView()));

    viewActionGroup = new QActionGroup(this);
    viewActionGroup->addAction(tableViewAction);
    viewActionGroup->addAction(listViewAction);

    showHiddenAction = new QAction("Show Hidden", this);
    showHiddenAction->setCheckable(true);
    connect(showHiddenAction, SIGNAL(triggered()), this, SLOT(slotShowHidden()));

    findAction = new QAction(QIcon::fromTheme("find", QIcon(":/Images/Find.png")), "Find", this);
    findAction->setShortcuts(QKeySequence::Find);
    connect(findAction, SIGNAL(triggered()), this, SLOT(slotShowFindWindow()));

    fileMenu = menuBar->addMenu("File");
    fileMenu->addAction(deleteAction);
    fileMenu->addAction(newFolderAction);
    fileMenu->addSeparator();
    fileMenu->addAction(settingsAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    editMenu = menuBar->addMenu("Edit");
    editMenu->addAction(copyAction);
    editMenu->addAction(cutAction);
    editMenu->addAction(pasteAction);

    viewMenu = menuBar->addMenu("View");
    viewMenu->addAction(tableViewAction);
    viewMenu->addAction(listViewAction);
    viewMenu->addAction(showHiddenAction);

    helpMenu = menuBar->addMenu("Help");
    helpMenu->addAction(aboutAction);

    setMenuBar(menuBar);

    toolBar = addToolBar("Tools");
    toolBar->addAction(tableViewAction);
    toolBar->addAction(listViewAction);
    toolBar->addSeparator();
    toolBar->addAction(findAction);

    contextMenu = new QMenu(this);
    contextMenu->addAction(tableViewAction);
    contextMenu->addAction(listViewAction);
    contextMenu->addSeparator();
    contextMenu->addAction(copyAction);
    contextMenu->addAction(cutAction);
    contextMenu->addAction(pasteAction);
    contextMenu->addSeparator();
    contextMenu->addAction(newFolderAction);
    contextMenu->addAction(deleteAction);
    contextMenu->addSeparator();
    contextMenu->addAction(propertiesAction);
}

void MainWindow::slotShowAbout()
{
    QMessageBox::about(this, "About FileINA", "<h2>FileINA</h2>"
                       "<p><em>Version 1.0</em>"
                       "<p>File Manager<br>"
                       "2015 by Alexey Belmach<br>");
}

void MainWindow::slotNewFolder()
{
    QAbstractItemView *currentView = getActivePane()->getCurrentWidget();
    int newFolderNum = 0;
    QModelIndex newFolder;

    do
    {
        if (!newFolderNum)
            newFolder = fileSystemModel->mkdir(currentView->rootIndex(), QString("New Folder"));
        else
            newFolder = fileSystemModel->mkdir(currentView->rootIndex(), QString("New Folder(%1)").arg(newFolderNum));
        newFolderNum++;
    }while(!newFolder.isValid());

    currentView->selectionModel()->setCurrentIndex(newFolder, QItemSelectionModel::ClearAndSelect);
    currentView->edit(newFolder);
}

void MainWindow::slotSettings()
{
    Settings settings(this);
    settings.exec();
}

void MainWindow::slotDelete()
{
    QModelIndexList selectionList;
    bool yesToAll = false;
    bool confirm = true;

    QWidget* focus(focusWidget());
    QAbstractItemView* view;
    if (viewIsFocused(focus, false))
    {
        view = (QAbstractItemView *)focus;
        selectionList = view->selectionModel()->selectedIndexes();
    }
    else
        return;

    quint64 toAdd = getActivePane()->getCurrentView() == Pane::Table ? 4 : 1;
    for (int i = 0; i < selectionList.count(); i += toAdd)
    {
        QFileInfo fileInfo(fileSystemModel->filePath(selectionList.at(i)));
        if (fileInfo.fileName() == ".." || fileInfo.fileName() == ".")
            continue;
        if (fileInfo.isSymLink())
        {
            DeleteThread *thread = new DeleteThread(fileInfo);
            connect(thread, SIGNAL(DeleteError()), this, SLOT(slotDeleteError()));
            thread->start();
        }
        else
        {
            if (!yesToAll)
                if (confirm)
                {
                    int answer = QMessageBox::information(this, "Delete file", "Are you sure you want to delete<p><b>\"" + fileInfo.filePath() + "\"</b>?", QMessageBox::Yes | QMessageBox::Cancel | QMessageBox::YesToAll);
                    if (answer == QMessageBox::YesToAll)
                        yesToAll = true;
                    if (answer == QMessageBox::Cancel)
                        return;
                }
            DeleteThread *thread = new DeleteThread(selectionList.at(i), fileSystemModel, fileInfo);
            connect(thread, SIGNAL(DeleteError()), this, SLOT(slotDeleteError()));
            thread->start();
        }
    }
}

void MainWindow::slotDeleteError()
{
    QMessageBox::information(this, "Delete failed", "Some files could not be deleted.");
}

void MainWindow::slotCopy()
{
    QModelIndexList selectionList;
    QWidget* focus(focusWidget());
    QAbstractItemView* view;

    if (getActivePane()->isFocused(focus, false))
    {
        view = (QAbstractItemView *)focus;
        selectionList = view->selectionModel()->selectedIndexes();
    }
    if (selectionList.count() == 0)
        return;

    QApplication::clipboard()->setMimeData(fileSystemModel->mimeData(selectionList));

    pasteAction->setData(false);
}

void MainWindow::slotCut()
{
    QModelIndexList selectionList;
    QWidget* focus(focusWidget());
    QAbstractItemView* view;

    if (getActivePane()->isFocused(focus, false))
    {
        view = (QAbstractItemView *)focus;
        selectionList = view->selectionModel()->selectedIndexes();
    }
    if (selectionList.count() == 0)
        return;

    QApplication::clipboard()->setMimeData(fileSystemModel->mimeData(selectionList));
    pasteAction->setData(true);
    view->selectionModel()->clear();
}

void MainWindow::slotPaste()
{
    QWidget* focus(focusWidget());
    Qt::DropAction copyOrCut(pasteAction->data().toBool() ? Qt::MoveAction : Qt::CopyAction);

    if (getActivePane()->isFocused(focus, false))
        if (copyOrCut == Qt::CopyAction)
        {
            CopyProgress *progress = new CopyProgress();
            CopyThread *thread = new CopyThread(getActivePane()->getPath(), fileSystemModel);
            connect(thread, SIGNAL(setMaxTotal(quint64)), progress, SLOT(slotSetMaxTotal(quint64)));
            connect(thread, SIGNAL(setCurrentTotal(quint64)), progress, SLOT(slotSetCurrentTotal(quint64)));
            connect(thread, SIGNAL(setFromTo(QString,QString)), progress, SLOT(slotSetFromTo(QString, QString)));
            connect(thread, SIGNAL(setMaxSize(quint64, quint64)), progress, SLOT(slotSetMaxSize(quint64, quint64)));
            connect(thread, SIGNAL(setCurrentSize(quint64)), progress, SLOT(slotSetCurrentSize(quint64)));
            connect(thread, SIGNAL(closeProgressBar()), progress, SLOT(deleteLater()));
            connect(progress, SIGNAL(abortCopy()), thread, SLOT(slotAbort()));
            thread->start();
            progress->show();
        }
        else
        {
            PasteThread *thread = new PasteThread(copyOrCut, qobject_cast<QAbstractItemView *>(focus)->rootIndex(), fileSystemModel);
            thread->start();
        }
    else
        if (focus == dirTreeView && copyOrCut == Qt::MoveAction)
        {
            PasteThread *thread = new PasteThread(copyOrCut, fileSystemProxyModel->mapToSource(dirTreeView->currentIndex()), fileSystemModel);
            thread->start();
        }
}

void MainWindow::slotClipboardChanged()
{
    if (QApplication::clipboard()->mimeData()->hasUrls())
        pasteAction->setEnabled(true);
    else
        pasteAction->setEnabled(false);
}

void MainWindow::showContextMenu(QPoint pos)
{
    contextMenu->exec(pos);
}

void MainWindow::slotTableView()
{
    activePane->setViewMode(Pane::Table);
}

void MainWindow::slotListView()
{
    activePane->setViewMode(Pane::List);
}

void MainWindow::slotShowProperties()
{
    if (getActivePane()->getPath().isEmpty())
        return;
    if (((QAbstractItemView *)focusWidget())->selectionModel()->selectedIndexes().count() > 0)
    {
        Properties prop("", this);
        QTimer::singleShot(100, &prop, SLOT(slotDirSize()));
        prop.exec();
    }
    else
    {
        Properties prop(getActivePane()->getPath(), this);
        QTimer::singleShot(100, &prop, SLOT(slotDirSize()));
        prop.exec();
    }
}

void MainWindow::slotShowFindWindow()
{
    FindWindow *find = new FindWindow(getActivePane()->getPath());
    find->show();
}

void MainWindow::slotShowHidden()
{
    if (showHiddenAction->isChecked())
        fileSystemModel->setFilter(QDir::NoDot | QDir::AllEntries | QDir::System | QDir::Hidden);
    else
        fileSystemModel->setFilter(QDir::NoDot | QDir::AllEntries | QDir::System);
}

QFileSystemModel* MainWindow::getFileSystemModel()
{
    return fileSystemModel;
}

void MainWindow::saveSettings()
{
    settings->setValue("Geometry", saveGeometry());
    settings->setValue("ToolBar", toolBar->isVisible());
    settings->setValue("Splitter", splitter->saveState());
    settings->setValue("LeftPaneIsActive", leftPane->isActive());
    settings->setValue("LeftPanePath", leftPane->getPath());
    settings->setValue("LeftPaneListHeader", leftPane->getHeader()->saveState());
    settings->setValue("LeftPaneViewMode", leftPane->getStackedWidget()->currentIndex());
    settings->setValue("RightPanePath", rightPane->getPath());
    settings->setValue("RightPaneListHeader", rightPane->getHeader()->saveState());
    settings->setValue("RightPaneViewMode", rightPane->getStackedWidget()->currentIndex());
    settings->setValue("ShowHidden", showHiddenAction->isChecked());
}

void MainWindow::restoreSettings()
{
    restoreGeometry(settings->value("Geometry").toByteArray());
    toolBar->setVisible(settings->value("ToolBar", QVariant(false)).toBool());
    splitter->restoreState(settings->value("Splitter").toByteArray());
    setActivePane(settings->value("LeftPaneIsActive", 1).toBool() ? leftPane : rightPane);
    leftPane->changeTo(settings->value("LeftPanePath", "").toString());
    leftPane->getTreeView()->header()->restoreState(settings->value("LeftPaneListHeader").toByteArray());
    leftPane->setViewMode(settings->value("LeftPaneViewMode", 0).toInt());
    rightPane->changeTo(settings->value("RightPanePath", "").toString());
    rightPane->getTreeView()->header()->restoreState(settings->value("RightPaneListHeader").toByteArray());
    rightPane->setViewMode(settings->value("RightPaneViewMode", 0).toInt());
    showHiddenAction->setChecked(settings->value("ShowHidden", false).toBool());
    slotShowHidden();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    saveSettings();
    event->accept();
}

bool FileSystemFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
    QFileSystemModel *fileSystemModel = (QFileSystemModel *) sourceModel();

    if ((fileSystemModel->isDir(index)) && (fileSystemModel->fileName(index).compare("..") != 0))
        return true;
    else
        return false;
}

MainWindow::~MainWindow()
{
}
