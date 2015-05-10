#ifndef COPYTHREAD_H
#define COPYTHREAD_H

#include <QThread>
#include <QDir>
#include <QDirIterator>
#include <QFileSystemModel>
#include <QModelIndex>
#include <QApplication>
#include <QClipboard>
#include <QMimeData>
#include <QVector>

class CopyThread : public QThread
{
    Q_OBJECT
    QModelIndex focus;
    QFileSystemModel *fileSystemModel;
    QString strTo;
    QVector<QString> *copyVector;
    quint64 totalSize, currentTotalSize;

public:
    explicit CopyThread(QString path, QFileSystemModel *model, QModelIndex focus, QObject *parent = 0);
    virtual void run();

signals:
    void setMaxTotal(quint64);
    void setCurrentTotal(quint64);
    void setFromTo(QString, QString);
    void setMaxSize(quint64, quint64);
    void setCurrentSize(quint64);
    void closeProgressBar();
};

#endif // COPYTHREAD_H
