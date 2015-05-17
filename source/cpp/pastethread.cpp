#include "pastethread.h"

PasteThread::PasteThread(Qt::DropAction cutOrCopy, QModelIndex focus, QFileSystemModel *model, QObject *parent) :
    QThread(parent)
{
    copyOrCut = cutOrCopy;
    this->focus = focus;
    fileSystemModel = model;
}

void PasteThread::run()
{
    fileSystemModel->dropMimeData(QApplication::clipboard()->mimeData(), copyOrCut, 0, 0, focus);
}

