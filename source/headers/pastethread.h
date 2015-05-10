#ifndef PASTETHREAD_H
#define PASTETHREAD_H

#include <QThread>
#include <QModelIndex>
#include <QFileSystemModel>
#include <QApplication>
#include <QClipboard>

class PasteThread : public QThread
{
    Q_OBJECT
    Qt::DropAction copyOrCut;
    QModelIndex focus;
    QFileSystemModel *fileSystemModel;
   /* QProgressDialog *progressDialog;
    QFutureWatcher<void> FutureWatcher;*/

public:
    PasteThread(Qt::DropAction cutOrCopy, QModelIndex focus, QFileSystemModel *model, QObject *parent = 0);
    virtual void run();
   // void Function();

/*private slots:
    void slotFinished();*/

};

#endif // PASTETHREAD_H
