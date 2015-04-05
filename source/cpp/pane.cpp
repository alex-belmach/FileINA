#include "pane.h"
#include "mainwindow.h"

Pane::Pane(QWidget *parent):
    QFrame(parent)
{
    mainWindow = (MainWindow*)parent->parent();

    pathLineEdit = new QLineEdit();
    connect(pathLineEdit, SIGNAL(editingFinished()), this, SLOT(slotPathChanged()));

    treeView = new QTreeView(this);
    treeView->setModel(mainWindow->fileSystemModel);
    treeView->setFrameStyle(QFrame::NoFrame | QFrame::Plain);
    treeView->setRootIsDecorated(false);
    treeView->setSelectionMode(QAbstractItemView::ExtendedSelection); //find simple selection
    treeView->setSortingEnabled(true); //think about unsynch sorting
    treeView->setEditTriggers(QAbstractItemView::SelectedClicked);
    connect(treeView, SIGNAL(activated(QModelIndex)), this, SLOT(slotDoubleClicked(QModelIndex)));

    listView = new QListView(this);
    listView->setModel(mainWindow->fileSystemModel);
    listView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    listView->setEditTriggers(QAbstractItemView::SelectedClicked);
    connect(listView, SIGNAL(activated(QModelIndex)), this, SLOT(slotDoubleClicked(QModelIndex)));

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
    QFileInfo fileInfo(mainWindow->fileSystemModel->filePath(index));
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

void Pane::changeTo(const QString &path)
{
    pathLineEdit->setText(path);
    QModelIndex index(mainWindow->fileSystemModel->index(path));
    treeView->setRootIndex(index);
    listView->setRootIndex(index);
    if (active)
        mainWindow->dirTreeView->setCurrentIndex(mainWindow->fileSystemProxyModel->mapFromSource(index));
}

void Pane::slotPathChanged()
{
    QFileInfo fileInfo(pathLineEdit->text());
    if (fileInfo.isDir())
        changeTo(pathLineEdit->text());
    else
        pathLineEdit->setText(mainWindow->fileSystemModel->filePath(treeView->rootIndex()));
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

bool Pane::isFocused(QWidget* focus)
{
    if (focus == pathLineEdit || focus == treeView || focus == listView)
        return true;
    else
        return false;
}

int Pane::currentView()
{
    return stackedWidget->currentIndex();
}
