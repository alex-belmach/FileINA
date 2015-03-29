#include "settings.h"
#include "mainwindow.h"

Settings::Settings(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle("Settings");

    layout = new QVBoxLayout(this);

    toolBar = ((MainWindow *)parent)->toolBar;
    toolBarCheckBox = new QCheckBox("Show Tool Bar");
    toolBarCheckBox->setChecked(toolBar->isVisible());

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(slotAccepted()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(slotRejected()));
    layout->addWidget(toolBarCheckBox);
    layout->addWidget(buttonBox);
    setLayout(layout);
}

void Settings::slotAccepted()
{
    toolBar->setVisible(toolBarCheckBox->isChecked());
    done(1);
}

void Settings::slotRejected()
{
    done(0);
}

