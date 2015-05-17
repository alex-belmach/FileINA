#include "settings.h"

Settings::Settings(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle("Settings");

    layout = new QVBoxLayout(this);

    mainWindow = (MainWindow *) parent;

    toolBarCheckBox = new QCheckBox("Show Tool Bar");
    toolBarCheckBox->setChecked(mainWindow->toolBarVisibility());

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(slotAccepted()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(slotRejected()));

    layout->addWidget(toolBarCheckBox);
    layout->addWidget(buttonBox);
    setLayout(layout);
}

void Settings::slotAccepted()
{
    mainWindow->toolBarSetVisible(toolBarCheckBox->isChecked());
    done(1);
}

void Settings::slotRejected()
{
    done(0);
}

