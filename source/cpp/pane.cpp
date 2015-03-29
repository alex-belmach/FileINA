#include "pane.h"

Pane::Pane(QWidget *parent):
    QFrame(parent)
{
    mainWindow = (QMainWindow *)parent->parent();

    pathLineEdit = new QLineEdit();

    treeView = new QTreeView(this);

    listView = new QListView(this);

    stackedWidget = new QStackedWidget();
    stackedWidget->addWidget(treeView);
    stackedWidget->addWidget(listView);

    vBoxLayout = new QVBoxLayout(this);
    vBoxLayout->addWidget(pathLineEdit);
    vBoxLayout->addWidget(stackedWidget);
    vBoxLayout->setSpacing(0);
    setLayout(vBoxLayout);
    setFrameStyle(QFrame::Box | QFrame::Plain);
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
