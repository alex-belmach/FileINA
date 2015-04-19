#include "pane.h"
#include "mainwindow.h"

Pane::Pane(QWidget *parent):
    QFrame(parent)
{
    mainWindow = (MainWindow*)parent->parent();

    pathLineEdit = new QLineEdit();
    connect(pathLineEdit, SIGNAL(editingFinished()), this, SLOT(slotPathChanged()));

    treeView = new QTreeView(this);
    treeView->setModel(mainWindow->getFileSystemModel());
    treeView->setFrameStyle(QFrame::NoFrame | QFrame::Plain);
    treeView->setRootIsDecorated(false);
    treeView->setSelectionMode(QAbstractItemView::ExtendedSelection); //find simple selection
    //treeView->setSortingEnabled(true); //think about unsynch sorting
    treeView->setEditTriggers(QAbstractItemView::SelectedClicked);
    connect(treeView, SIGNAL(activated(QModelIndex)), this, SLOT(slotDoubleClicked(QModelIndex)));
    treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(treeView, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(slotContextMenu(const QPoint&)));

    listView = new QListView(this);
    listView->setModel(mainWindow->getFileSystemModel());
    listView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    listView->setEditTriggers(QAbstractItemView::SelectedClicked);
    connect(listView, SIGNAL(activated(QModelIndex)), this, SLOT(slotDoubleClicked(QModelIndex)));
    listView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(listView, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(slotContextMenu(const QPoint&)));

    stackedWidget = new QStackedWidget();
    stackedWidget->addWidget(treeView);
    stackedWidget->addWidget(listView);

    vBoxLayout = new QVBoxLayout(this);
    vBoxLayout->addWidget(pathLineEdit);
    vBoxLayout->addWidget(stackedWidget);
    vBoxLayout->setSpacing(0);
    vBoxLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(vBoxLayout);
    setFrameStyle(QFrame::Box | QFrame::Plain);
}

void Pane::slotDoubleClicked(QModelIndex index)
{
    QFileInfo fileInfo(mainWindow->getFileSystemModel()->filePath(index));
    if (fileInfo.isDir())
        changeTo(fileInfo.absoluteFilePath());
    else
        if (fileInfo.isExecutable())
        {
            QProcess *process = new QProcess(this);
            process->startDetached(fileInfo.absoluteFilePath());
        }
    else //doesn't work
        {
            QProcess *process = new QProcess(this);
            process->startDetached("\"" + fileInfo.absoluteFilePath() + "\"");
        }
}

void Pane::slotContextMenu(const QPoint& pos)
{
    mainWindow->showContextMenu(listView->mapToGlobal(pos));
}

void Pane::changeTo(const QString &path)
{
    pathLineEdit->setText(path);
    QModelIndex index(mainWindow->getFileSystemModel()->index(path));
    treeView->setRootIndex(index);
    listView->setRootIndex(index);
    if (active)
        mainWindow->setCurrentIndex(index);
}

void Pane::slotPathChanged()
{
    QFileInfo fileInfo(pathLineEdit->text());
    if (fileInfo.isDir())
        changeTo(pathLineEdit->text());
    else
        pathLineEdit->setText(mainWindow->getFileSystemModel()->filePath(treeView->rootIndex()));
}

void Pane::setActive(bool active)
{
    if (this->active == active) return;

    this->active = active;
    treeView->setAlternatingRowColors(active);
    listView->setAlternatingRowColors(active);
}

void Pane::setViewMode(const ViewMode view)
{
    stackedWidget->setCurrentIndex(view);
}

bool Pane::isFocused(QWidget* focus, bool withPath)
{
    if (withPath)
        if (focus == pathLineEdit || focus == treeView || focus == listView)
            return true;
        else
            return false;
    else
        if (focus == treeView || focus == listView)
            return true;
        else
            return false;
}

QAbstractItemView* Pane::getCurrentWidget()
{
    return (QAbstractItemView *)stackedWidget->currentWidget();
}

int Pane::getCurrentView()
{
    return stackedWidget->currentIndex();
}
