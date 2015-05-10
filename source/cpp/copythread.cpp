#include "copythread.h"
#include <QDebug>

#define USELESS_DATA_NUM 8
#define PROGRESS_BAR_UPDATE_COUNT 2048
#define INT32_MAX 2147483647


CopyThread::CopyThread(QString path, QFileSystemModel *model, QModelIndex focus, QObject *parent) :
    QThread(parent)
{
    strTo = path;
    this->focus = focus;
    fileSystemModel = model;
    if (strTo.right(1) != "/")
        strTo += '/';
    copyVector = new QVector<QString>();
    totalSize = 0;
    currentTotalSize = 0;
}

void CopyThread::run()
{
    QString copyData = QApplication::clipboard()->mimeData()->text();
    while (copyData.length() > USELESS_DATA_NUM)
    {
        copyVector->append("");
        copyData.remove(0, USELESS_DATA_NUM);
        int i = 0;
        while (!copyData[i].isNull() && copyData[i] != '\n')
            copyVector->last().append(copyData[i++]);
        copyData.remove(0, i + 1);
        QFileInfo *fileInfo = new QFileInfo(copyVector->last());
        if (fileInfo->isDir())
        {
            QDir dir(copyVector->last());
            totalSize += dir.count();
        }
        else
            totalSize++;

    }
    setMaxTotal(totalSize);
    for (int i = 0; i < copyVector->count(); i++)
    {
        QString strFrom = copyVector->at(i);
        QFileInfo *fileInfo = new QFileInfo(strFrom);
        if (!fileInfo->isDir())
        {
            QFile fileFrom(fileInfo->absoluteFilePath());
            QFile fileTo(strTo + fileInfo->fileName());
            setFromTo(fileInfo->absoluteFilePath(), strTo + fileInfo->fileName());
            quint64 size = fileInfo->size(), ratio = 1;
            if (size > INT32_MAX)
                ratio = size / INT32_MAX + 1;
            setMaxSize(fileInfo->size(), ratio);
            fileFrom.open(QIODevice::ReadOnly);
            fileTo.open(QIODevice::WriteOnly);
            quint64 bytesCopied, totalBytesCopied = 0, num = 0;
            while (!fileFrom.atEnd())
            {
                num++;
                bytesCopied = fileTo.write(fileFrom.readLine());
                totalBytesCopied += bytesCopied;
                if (num % PROGRESS_BAR_UPDATE_COUNT == 0)
                {
                    setCurrentSize(totalBytesCopied);
                }
            }
            fileFrom.close();
            fileTo.close();
            setCurrentTotal(++currentTotalSize);
        }
        else
        {
            QDir dir;
            if (strFrom.right(1) != "/")
                strFrom += '/';
            QFileInfo *info = new QFileInfo(strFrom);
            quint64 pathNum = info->canonicalPath().length();
            dir.mkdir(strTo + strFrom.right(strFrom.length() - pathNum));
            QDirIterator itr(strFrom, QDirIterator::Subdirectories);

            while (itr.hasNext())
            {
                QString filePath = itr.next();
                QString fileName = itr.fileName();
                if ((fileName != ".") && (fileName != "..") && (fileName != ""))
                {
                    if (itr.fileInfo().isDir())
                    {
                        QString newDir = strTo + filePath.right(filePath.length() - pathNum);
                        dir.mkdir(newDir);
                    }
                    else
                    {
                        QFile fileFrom(filePath);
                        QString toPath = strTo + filePath.right(filePath.length() - pathNum);
                        QFile fileTo(toPath);
                        setFromTo(filePath, toPath);
                        quint64 size = fileFrom.size(), ratio = 1;
                        if (size > INT32_MAX)
                            ratio = size / INT32_MAX + 1;
                        setMaxSize(fileFrom.size(), ratio);
                        fileFrom.open(QIODevice::ReadOnly);
                        fileTo.open(QIODevice::WriteOnly);
                        quint64 bytesCopied, totalBytesCopied = 0, num = 0;
                        while (!fileFrom.atEnd())
                        {
                            num++;
                            bytesCopied = fileTo.write(fileFrom.readLine());
                            totalBytesCopied += bytesCopied;
                            if (num % PROGRESS_BAR_UPDATE_COUNT == 0)
                            {
                                setCurrentSize(totalBytesCopied);
                            }
                        }
                        fileFrom.close();
                        fileTo.close();
                    }
                }
                setCurrentTotal(++currentTotalSize);
            }
        }
    }
    closeProgressBar();
    return;
}
