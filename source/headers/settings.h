#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include <QCheckBox>
#include <QToolBar>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QDialog>

class Settings : public QDialog
{
    Q_OBJECT
public:
    explicit Settings(QWidget *parent = 0);

private:
    QVBoxLayout * layout;
    QCheckBox *toolBarCheckBox;
    QToolBar *toolBar;
    QDialogButtonBox *buttonBox;

private slots:
    void slotAccepted();
    void slotRejected();
};

#endif // SETTINGS_H
