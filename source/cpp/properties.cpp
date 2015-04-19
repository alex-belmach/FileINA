#include "properties.h"
#include "mainwindow.h"
#include "pane.h"
#include <QFormLayout>
#include <QLabel>
#include <QDateTime>

Properties::Properties(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle("Properties");
    QVBoxLayout *layout = new QVBoxLayout(this);
    QFormLayout *formLayout = new QFormLayout();
    formLayout->setMargin(5);

    MainWindow *mainWindow((MainWindow *)parent);
    QWidget *focus(mainWindow->focusWidget());
    QAbstractItemView *view;
    if (mainWindow->viewIsFocused(focus, false))
    {
        view = (QAbstractItemView *)focus;
        QFileInfo fileInfo = mainWindow->getFileSystemModel()->fileInfo(view->selectionModel()->selectedIndexes().at(0));
        QLabel *nameLabel = new QLabel(fileInfo.fileName());
        formLayout->addRow("Name:", nameLabel);
        QLabel *pathLabel = new QLabel(fileInfo.path());
        formLayout->addRow("Path:", pathLabel);
        QLabel *sizeLabel = new QLabel(QString::number(fileInfo.size() / 1024) + " Kb (" + QString::number(fileInfo.size()) + " bytes)");
        formLayout->addRow("Size:", sizeLabel); //for dirs and Gb, Mb etc..
        QLabel *createdLabel = new QLabel(fileInfo.created().toString());
        formLayout->addRow("Created:", createdLabel);
        QLabel *modifiedLabel = new QLabel(fileInfo.lastModified().toString());
        formLayout->addRow("Last Modified:", modifiedLabel);
        QLabel *readLabel = new QLabel(fileInfo.lastRead().toString());
        formLayout->addRow("Last Read:", readLabel);
    }
    QDialogButtonBox *buttonOk = new QDialogButtonBox(QDialogButtonBox::Ok);
    connect(buttonOk, SIGNAL(accepted()), this, SLOT(slotAccepted()));
    layout->addLayout(formLayout);
    layout->addWidget(buttonOk);
    setLayout(layout);
}

void Properties::slotAccepted()
{
    done(1);
}
