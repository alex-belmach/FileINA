#include "properties.h"

#define BYTES_IN_KB 1024.0
#define BYTES_IN_MB 1048576.0
#define BYTES_IN_GB 1073741824.0
#define SIZE_NUMS 4
#define UPDATE_TIME 500

Properties::Properties(QString path, QWidget *parent) :
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
        if (path == "")
            fileInfo = mainWindow->getFileSystemModel()->fileInfo(view->selectionModel()->selectedIndexes().at(0));
        else
            fileInfo = QFileInfo(path);

        QLabel *nameLabel = new QLabel(fileInfo.fileName());
        formLayout->addRow("Name:", nameLabel);

        QLabel *pathLabel = new QLabel(fileInfo.path());
        formLayout->addRow("Path:", pathLabel);

        quint64 size = 0;
        if (!fileInfo.isDir())
            size = fileInfo.size();
        if (size < BYTES_IN_KB)
            sizeLabel = new QLabel(QString::number(size) + " bytes");
        else
            if (size < BYTES_IN_MB)
                sizeLabel = new QLabel(QString::number(size / BYTES_IN_KB, 'g', SIZE_NUMS) + " Kb (" + QString::number(size) + " bytes)");
            else
                if (size < BYTES_IN_GB)
                    sizeLabel = new QLabel(QString::number(size / BYTES_IN_MB, 'g', SIZE_NUMS) + " Mb (" + QString::number(size) + " bytes)");
                else
                    sizeLabel = new QLabel(QString::number(size / BYTES_IN_GB, 'g', SIZE_NUMS) + " Gb (" + QString::number(size) + " bytes)");
        formLayout->addRow("Size:", sizeLabel);

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
        thread = new SizeThread(fileInfo);
        connect(thread, SIGNAL(changeLabel(QString)), this, SLOT(slotChangeLabel(QString)));
        connect(this, SIGNAL(finished(int)), thread, SLOT(slotExit()));
        connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

        timer = new QTimer;
        connect(timer, SIGNAL(timeout()), thread, SLOT(slotSendSize()));
        connect(thread, SIGNAL(stopTimer()), timer, SLOT(stop()));

        timer->start(UPDATE_TIME);
        thread->start();
    }
}

void Properties::slotChangeLabel(QString sizeText)
{
    sizeLabel->setText(sizeText);
}

void Properties::slotAccepted()
{
    done(1);
}
