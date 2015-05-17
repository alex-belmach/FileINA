#include "sizethread.h"

#define BYTES_IN_KB 1024.0
#define BYTES_IN_MB 1048576.0
#define BYTES_IN_GB 1073741824.0
#define SIZE_NUMS 4

SizeThread::SizeThread(QFileInfo info, QObject *parent) :
    QThread(parent)
{
    fileInfo = info;
    exitFlag = false;
}

void SizeThread::run()
{
    result = 0;

    QDirIterator it(fileInfo.absoluteFilePath(), QDirIterator::Subdirectories);
    while (it.hasNext() && !exitFlag)
    {
        QFileInfo itFileInfo(it.next());
        if (itFileInfo.isFile())
            result += itFileInfo.size();
    }

    slotSendSize();
    emit stopTimer();
}

void SizeThread::slotSendSize()
{
    QString size;
    if (result < BYTES_IN_KB)
        size = QString::number(result) + " bytes";
    else
        if (result < BYTES_IN_MB)
            size = QString::number((double)(result / BYTES_IN_KB), 'g', SIZE_NUMS) + " Kb (" + QString::number(result) + " bytes)";
        else
            if (result < BYTES_IN_GB)
                size = QString::number((double)(result / BYTES_IN_MB), 'g', SIZE_NUMS) + " Mb (" + QString::number(result) + " bytes)";
            else
                size = QString::number((double)(result / BYTES_IN_GB), 'g', SIZE_NUMS) + " Gb (" + QString::number(result) + " bytes)";
    emit changeLabel(size);
}

void SizeThread::slotExit()
{
    exitFlag = true;
}
