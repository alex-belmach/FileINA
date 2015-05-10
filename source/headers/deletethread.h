#ifndef DELETETHREAD_H
#define DELETETHREAD_H

#include <QThread>
#include <QModelIndexList>
#include <QFileSystemModel>
#include <QMessageBox>

class DeleteThread : public QThread
{
    Q_OBJECT
    QFileSystemModel *fileSystemModel;
    QModelIndex selectionIndex;
    QFileInfo fileInfo;
    bool isSymLink;

public:
    DeleteThread(QModelIndex index, QFileSystemModel *model, QFileInfo info, QObject *parent = 0);
    DeleteThread(QFileInfo info, QObject *parent = 0);
    virtual void run();

signals:
    void DeleteError();
};

#endif // DELETETHREAD_H
