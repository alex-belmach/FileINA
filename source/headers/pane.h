#ifndef PANE_H
#define PANE_H

#include <QFrame>
#include <QLineEdit>
#include <QTreeView>
#include <QListView>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QProcess>
#include <QUrl>
#include <QDesktopServices>

class MainWindow;

class Pane: public QFrame
{
    Q_OBJECT

public:
    enum ViewMode
    {
        Table,
        List
    };

    explicit Pane(QWidget* parent = 0);

    bool isFocused(QWidget *, bool);
    bool isActive();
    int getCurrentView();
    void setActive(bool);
    void setViewMode(const ViewMode);
    void setViewMode(const int);
    void changeTo(const QString &);
    QAbstractItemView* getCurrentWidget();
    QHeaderView* getHeader();
    QStackedWidget* getStackedWidget();
    QString getPath();
    QTreeView* getTreeView();

private:
    bool active;
    MainWindow *mainWindow;
    QLineEdit *pathLineEdit;
    QListView *listView;
    QStackedWidget *stackedWidget;
    QTreeView *treeView;
    QVBoxLayout *vBoxLayout;

private slots:
    void slotDoubleClicked(QModelIndex);
    void slotPathChanged();
    void slotContextMenu(const QPoint &);
};

#endif // PANE_H
