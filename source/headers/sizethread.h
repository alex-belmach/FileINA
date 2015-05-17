#ifndef SIZETHREAD_H
#define SIZETHREAD_H

#include <QThread>
#include <QFileInfo>
#include <QDirIterator>
#include <QTimer>

class SizeThread : public QThread
{
    Q_OBJECT

    bool exitFlag;
    quint64 result;
    QFileInfo fileInfo;

public:
    explicit SizeThread(QFileInfo info, QObject *parent = 0);
    virtual void run();

signals:
    void changeLabel(QString);
    void stopTimer();

private slots:
    void slotSendSize();
    void slotExit();
};

#endif // SIZETHREAD_H
