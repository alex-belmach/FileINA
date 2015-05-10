#include "pastethread.h"
#include <QDebug>
#include <QMimeData>


PasteThread::PasteThread(Qt::DropAction cutOrCopy, QModelIndex focus, QFileSystemModel *model, QObject *parent) :
    QThread(parent)
{
    copyOrCut = cutOrCopy;
    this->focus = focus;
    fileSystemModel = model;
    /*progressDialog = new QProgressDialog();
    connect(&this->FutureWatcher, SIGNAL (finished()), this, SLOT (slotFinished()));
    connect(&this->FutureWatcher, SIGNAL (finished()), this->progressDialog , SLOT (cancel()));*/
}

/*void PasteThread::slotFinished()
{
    this->quit();
}*/

void PasteThread::run()
{
    /*QFuture<void> future = QtConcurrent::run(this, &PasteThread::Function);
    this->FutureWatcher.setFuture(future);

    this->progressDialog->setMinimum(0);
     this->progressDialog->setMaximum(1);
     this->progressDialog->setWindowModality(Qt::WindowModal);
     this->progressDialog->exec();*/
    fileSystemModel->dropMimeData(QApplication::clipboard()->mimeData(), copyOrCut, 0, 0, focus);
    exec();
}

/*void PasteThread::Function()
{
    fileSystemModel->dropMimeData(QApplication::clipboard()->mimeData(), copyOrCut, 0, 0, focus);
}*/
