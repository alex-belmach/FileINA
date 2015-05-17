#ifndef FINDWINDOW_H
#define FINDWINDOW_H

#include <QWidget>
#include <QDir>
#include <QFileDialog>
#include <QProgressDialog>
#include <QDesktopServices>
#include <QHeaderView>
#include <QDirIterator>

namespace Ui
{
    class FindWindow;
}

class FindWindow : public QWidget
{
    Q_OBJECT

public:
    explicit FindWindow(QString ,QWidget *parent = 0);
    ~FindWindow();

private:
    Ui::FindWindow *ui;
    QDir currentDir;

    void showFiles(const QFileInfoList &);
    QFileInfoList findFiles(const QStringList &, const QString &);

private slots:
    void slotBrowse();
    void slotFind();
    void slotOpenFile(int, int);
};

#endif // FINDWINDOW_H
