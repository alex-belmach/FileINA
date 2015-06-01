#ifndef PROPERTIES_H
#define PROPERTIES_H

#include "sizethread.h"
#include "mainwindow.h"
#include "pane.h"
#include <QFormLayout>
#include <QDateTime>
#include <QDialog>
#include <QDir>
#include <QLabel>
#include <QTimer>

class Properties : public QDialog
{
    Q_OBJECT

    quint64 result;
    SizeThread *thread;
    QFileInfo fileInfo;
    QLabel *sizeLabel;
    QTimer *timer;

public:
    explicit Properties(QString path = "", QWidget *parent = 0);

private slots:
    void slotAccepted();
    void slotDirSize();
    void slotChangeLabel(QString);
};

#endif // PROPERTIES_H
