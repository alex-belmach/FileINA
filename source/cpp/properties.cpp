#include "properties.h"
#include "mainwindow.h"
#include "pane.h"
#include <QFormLayout>
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
        fileInfo = mainWindow->getFileSystemModel()->fileInfo(view->selectionModel()->selectedIndexes().at(0));
        QLabel *nameLabel = new QLabel(fileInfo.fileName());
        formLayout->addRow("Name:", nameLabel);
        QLabel *pathLabel = new QLabel(fileInfo.path());
        formLayout->addRow("Path:", pathLabel);
        quint64 size = 0;
        if (!fileInfo.isDir())
            size = fileInfo.size();
        if (size < 1024)
            sizeLabel = new QLabel(QString::number(size) + " bytes");
        else
            if (size < 1048576)
                sizeLabel = new QLabel(QString::number(size / 1024) + " Kb (" + QString::number(size) + " bytes)");
            else
                if (size < 1073741824)
                    sizeLabel = new QLabel(QString::number(size / 1048576) + " Mb (" + QString::number(size) + " bytes)");
                else
                    sizeLabel = new QLabel(QString::number(size / 1073741824) + " Gb (" + QString::number(size) + " bytes)");
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

void Properties::slotDirSize()
{
    if (fileInfo.isDir())
    {
        result = 0;
        QDirIterator it(fileInfo.absoluteFilePath(), QDirIterator::Subdirectories);
        while (it.hasNext())
        {
            QFileInfo fileInfo(it.next());
            if (fileInfo.isFile())
            result += fileInfo.size();
            if (result < 1024)
                sizeLabel->setText(QString::number(result) + " bytes");
            else
                if (result < 1048576)
                    sizeLabel->setText(QString::number((double)(result / 1024)) + " Kb (" + QString::number(result) + " bytes)");
                else
                    if (result < 1073741824)
                        sizeLabel->setText(QString::number((double)(result / 1048576)) + " Mb (" + QString::number(result) + " bytes)");
                    else
                        sizeLabel->setText(QString::number((double)(result / 1073741824)) + " Gb (" + QString::number(result) + " bytes)");
        }
    }
}

void Properties::slotAccepted()
{
    done(1);
}
