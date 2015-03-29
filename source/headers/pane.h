#ifndef PANE_H
#define PANE_H

#include <QFrame>
#include <QMainWindow>
#include <QLineEdit>
#include <QTreeView>
#include <QListView>
#include <QStackedWidget>
#include <QVBoxLayout>

class Pane: public QFrame
{
    Q_OBJECT
public:
    QLineEdit *pathLineEdit;
    QTreeView *treeView;
    QListView *listView;
    QStackedWidget *stackedWidget;

    enum ViewMode
    {
        Table, List
    };

    explicit Pane(QWidget* parent = 0);
    void setActive(bool );
    void setViewMode(const ViewMode view);

private:
    bool active;
    QMainWindow *mainWindow;
    QVBoxLayout *vBoxLayout;
};

#endif // PANE_H
