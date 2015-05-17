#include "deletethread.h"

DeleteThread::DeleteThread(QModelIndex index, QFileSystemModel *model, QFileInfo info, QObject *parent) :
    QThread(parent)
{
    fileSystemModel = model;
    selectionIndex = index;
    fileInfo = info;
    isSymLink = false;
}

DeleteThread::DeleteThread(QFileInfo info, QObject *parent):
    QThread(parent)
{
    fileInfo = info;
    isSymLink = true;
}

void DeleteThread::run()
{
    bool ok;
    QFile *file = new QFile(fileInfo.absoluteFilePath());

    file->open(QIODevice::WriteOnly);
    file->setPermissions(QFile::ReadOwner|QFile::WriteOwner|QFile::ExeOwner|QFile::ReadGroup|QFile::ExeGroup|QFile::ReadOther|QFile::ExeOther);
    file->close();

    if (isSymLink)
        ok = QFile::remove(fileInfo.filePath());
    else
        ok = fileSystemModel->remove(selectionIndex);
    if (!ok)
        emit DeleteError();
}
