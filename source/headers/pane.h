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
        Table, List
    };

    explicit Pane(QWidget* parent = 0);
    void setActive(bool );
    void setViewMode(const ViewMode );
    void setViewMode(const int );
    void changeTo(const QString& );
    bool isFocused(QWidget* , bool);
    int getCurrentView();
    QAbstractItemView* getCurrentWidget();
    bool isActive();
    QString getPath();
    QHeaderView* getHeader();
    QStackedWidget* getStackedWidget();
    QTreeView* getTreeView();

private:
    bool active;
    MainWindow *mainWindow;
    QVBoxLayout *vBoxLayout;
    QLineEdit *pathLineEdit;
    QTreeView *treeView;
    QListView *listView;
    QStackedWidget *stackedWidget;

private slots:
    void slotDoubleClicked(QModelIndex );
    void slotPathChanged();
    void slotContextMenu(const QPoint& );
};

#endif // PANE_H
