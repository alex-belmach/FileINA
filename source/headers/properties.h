#ifndef PROPERTIES_H
#define PROPERTIES_H

#include <QDialog>
#include <QDir>
#include <QLabel>
#include <QTimer>

class Properties : public QDialog
{
    Q_OBJECT

    QFileInfo fileInfo;
    QLabel *sizeLabel;
    quint64 result;

public:
    explicit Properties(QWidget *parent = 0);

private slots:
    void slotAccepted();
    void slotDirSize();
};

#endif // PROPERTIES_H
