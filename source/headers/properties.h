#ifndef PROPERTIES_H
#define PROPERTIES_H

#include <QDialog>

class Properties : public QDialog
{
    Q_OBJECT
public:
    explicit Properties(QWidget *parent = 0);

private slots:
    void slotAccepted();
};

#endif // PROPERTIES_H
