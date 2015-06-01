#include "copythread.h"

#ifdef __linux__
    #define USELESS_DATA_NUM 7
#elif _WIN32
    #define USELESS_DATA_NUM 8
#else

#endif
#define PROGRESS_BAR_UPDATE_COUNT 4096
#define INT32_MAX 2147483647

CopyThread::CopyThread(QString path, QFileSystemModel *model, QObject *parent) :
    QThread(parent)
{
    strTo = path;
    fileSystemModel = model;
    if (strTo.right(1) != "/")
        strTo += '/';
    copyVector = new QVector<QString>();
    totalSize = 0;
    currentTotalSize = 0;
    exitFlag = false;
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
    emit setMaxTotal(totalSize);

    for (int i = 0; i < copyVector->count(); i++)
    {
        if (exitFlag) break;
        QString strFrom = copyVector->at(i);
        QFileInfo *fileInfo = new QFileInfo(strFrom);
        if (!fileInfo->isDir())
        {
            QFile fileFrom(fileInfo->absoluteFilePath());
            toPath = strTo + fileInfo->fileName();
            if (fileInfo->absoluteFilePath() == toPath)
            {
                QFileInfo tempInfo(toPath);
                QString path = tempInfo.absolutePath();
                QString name = tempInfo.baseName() + " - copy(1)";
                QString suff = tempInfo.completeSuffix();
                quint64 copyNum = 1;

                bool fileExists;
                do
                {
                    QFile tempFile(path + "/" + name + "." + suff);
                    copyNum++;
                    name = name.left(name.length() - 1 - QString::number(copyNum - 1).length()) + QString::number(copyNum) + ")";
                    fileExists = tempFile.exists();
                } while (fileExists);
                copyNum--;
                toPath = path + "/" + name.left(name.length() - 1 - QString::number(copyNum + 1).length()) + QString::number(copyNum) + ")" + "." + suff;
            }
            QFile fileTo(toPath);
            emit setFromTo(fileInfo->absoluteFilePath(), toPath);
            quint64 size = fileInfo->size(), ratio = 1;
            if (size > INT32_MAX)
                ratio = size / INT32_MAX + 1;
            emit setMaxSize(fileInfo->size(), ratio);
            fileFrom.open(QIODevice::ReadOnly);
            fileTo.open(QIODevice::WriteOnly);
            quint64 bytesCopied, totalBytesCopied = 0, num = 0;
            while (!fileFrom.atEnd() && !exitFlag)
            {
                num++;
                bytesCopied = fileTo.write(fileFrom.readLine());
                totalBytesCopied += bytesCopied;
                if (num % PROGRESS_BAR_UPDATE_COUNT == 0)
                {
                    emit setCurrentSize(totalBytesCopied);
                }
            }
            fileFrom.close();
            fileTo.close();
            emit setCurrentTotal(++currentTotalSize);
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
            while (itr.hasNext() && !exitFlag)
            {
                filePath = itr.next();
                fileName = itr.fileName();
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
                        toPath = strTo + filePath.right(filePath.length() - pathNum);
                        QFile fileTo(toPath);
                        emit setFromTo(filePath, toPath);
                        quint64 size = fileFrom.size(), ratio = 1;
                        if (size > INT32_MAX)
                            ratio = size / INT32_MAX + 1;
                        emit setMaxSize(fileFrom.size(), ratio);

                        fileFrom.open(QIODevice::ReadOnly);
                        fileTo.open(QIODevice::WriteOnly);
                        quint64 bytesCopied, totalBytesCopied = 0, num = 0;
                        while (!fileFrom.atEnd() && !exitFlag)
                        {
                            num++;
                            bytesCopied = fileTo.write(fileFrom.readLine());
                            totalBytesCopied += bytesCopied;
                            if (num % PROGRESS_BAR_UPDATE_COUNT == 0)
                                emit setCurrentSize(totalBytesCopied);
                        }
                        fileFrom.close();
                        fileTo.close();
                    }
                }
                emit setCurrentTotal(++currentTotalSize);
            }
        }
    }
    if (exitFlag)
        QFile(toPath).remove();
    emit closeProgressBar();
}

void CopyThread::slotAbort()
{
    exitFlag = true;
}
