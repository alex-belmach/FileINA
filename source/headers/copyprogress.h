#ifndef COPYPROGRESS_H
#define COPYPROGRESS_H

#include <QWidget>

namespace Ui
{
    class CopyProgress;
}

class CopyProgress : public QWidget
{
    Q_OBJECT

public:
    explicit CopyProgress(QWidget *parent = 0);
    ~CopyProgress();

public slots:
    void slotSetMaxTotal(quint64);
    void slotSetCurrentTotal(quint64);
    void slotSetFromTo(QString, QString);
    void slotSetMaxSize(quint64, quint64);
    void slotSetCurrentSize(quint64);

signals:
    void abortCopy();

private:
    Ui::CopyProgress *ui;
    quint64 ratio;

private slots:
    void slotAbortCopy();
};

#endif // COPYPROGRESS_H
