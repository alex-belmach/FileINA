#ifndef PASTETHREAD_H
#define PASTETHREAD_H

#include <QThread>
#include <QModelIndex>
#include <QFileSystemModel>
#include <QApplication>
#include <QClipboard>
#include <QMimeData>

class PasteThread : public QThread
{
    Q_OBJECT

    QFileSystemModel *fileSystemModel;
    QModelIndex focus;
    Qt::DropAction copyOrCut;

public:
    PasteThread(Qt::DropAction cutOrCopy, QModelIndex focus, QFileSystemModel *model, QObject *parent = 0);
    virtual void run();
};

#endif // PASTETHREAD_H
