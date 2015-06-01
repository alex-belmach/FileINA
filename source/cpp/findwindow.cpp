#include "findwindow.h"
#include "ui_findwindow.h"

#define BYTES_IN_KB 1024

FindWindow::FindWindow(QString path, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FindWindow)
{
    ui->setupUi(this);
    setWindowTitle("Find Files");
    setWindowIcon(QIcon(":/Images/Find.png"));

    connect(ui->browseButton, SIGNAL(clicked()), this, SLOT(slotBrowse()));
    connect(ui->findButton, SIGNAL(clicked()), this, SLOT(slotFind()));
    connect(ui->cancelButton, SIGNAL(clicked()), this , SLOT(close()));

    ui->foundLabel->setText("");

    ui->fileComboBox->setCurrentText("*");
    ui->fileComboBox->setEditable(true);
    ui->fileComboBox->addItem("*");
    ui->fileComboBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    ui->dirComboBox->setCurrentText(path);
    ui->dirComboBox->setEditable(true);
    ui->dirComboBox->addItem(path);
    ui->dirComboBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    ui->textComboBox->setCurrentText("");
    ui->textComboBox->setEditable(true);
    ui->textComboBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    ui->filesTable->setColumnCount(2);
    ui->filesTable->setRowCount(0);
    ui->filesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    QStringList labels;
    labels << "File Name" << "Size";
    ui->filesTable->setHorizontalHeaderLabels(labels);
    ui->filesTable->setColumnWidth(0, 270);
    ui->filesTable->setColumnWidth(1, 82);
    ui->filesTable->verticalHeader()->hide();
    ui->filesTable->setShowGrid(false);
    connect(ui->filesTable, SIGNAL(cellActivated(int,int)), this, SLOT(slotOpenFile(int, int)));
}

void FindWindow::slotBrowse()
{
    QString directory = QFileDialog::getExistingDirectory(this, "Find Files", QDir::currentPath());
    if (!directory.isEmpty())
    {
        if (ui->dirComboBox->findText(directory) == -1)
            ui->dirComboBox->addItem(directory);
        ui->dirComboBox->setCurrentIndex(ui->dirComboBox->findText(directory));
    }
}

void FindWindow::slotFind()
{
    ui->filesTable->setRowCount(0);

    QString fileName = ui->fileComboBox->currentText();
    QString text = ui->textComboBox->currentText();
    QString path = ui->dirComboBox->currentText();
    if(path.isEmpty())
        return;
    QFileInfoList files;
    currentDir = QDir(path);
    fileName = "*" + fileName + "*";
    files = findFiles(QStringList(fileName), text);
    showFiles(files);
}

QFileInfoList FindWindow::findFiles(const QStringList &files, const QString &text)
{
    QDir dir(ui->dirComboBox->currentText());
    QString filePath;
    QFileInfoList foundFiles, fileList;
    ui->foundLabel->setText("Finding...");
    QDirIterator itr(dir, QDirIterator::Subdirectories);

    while (itr.hasNext())
    {
        filePath = itr.next();
        if (itr.fileInfo().isDir())
        {
            qApp->processEvents();
            QDir dir(filePath);
            fileList.append(dir.entryInfoList(files, QDir::Files | QDir::NoSymLinks));
        }
    }

    if (!text.isEmpty())
    {
        QProgressDialog progressDialog(this);
        progressDialog.setCancelButtonText("Cancel");
        progressDialog.setRange(0, fileList.size());
        progressDialog.setWindowTitle("Find Files");

        for (int j = 0; j < fileList.size(); j++)
        {
            if (progressDialog.wasCanceled())
                break;
            progressDialog.setValue(j);
            progressDialog.setLabelText(QString("Searching file number %1 of %2...").arg(j).arg(fileList.size()));
            qApp->processEvents();

            QFile file(fileList[j].absoluteFilePath());
            if (file.open(QIODevice::ReadOnly))
            {
                QString line;
                while (!file.atEnd())
                {
                    line = file.readLine();
                    if (line.contains(text))
                    {
                        foundFiles << fileList[j];
                        break;
                    }
                }
            }
        }
        return foundFiles;
    }
    else
        return fileList;
}

void FindWindow::showFiles(const QFileInfoList &files)
{
    ui->foundLabel->setText(QString("%1 file(s) found").arg(files.size()));

    for (int i = 0; i < files.size(); i++)
    {
        qApp->processEvents();
        QFile file(files[i].absoluteFilePath());
        quint64 size = file.size();

        QTableWidgetItem *nameItem = new QTableWidgetItem(files[i].absoluteFilePath().right(files[i].absoluteFilePath().length() - ui->dirComboBox->currentText().length()));
        nameItem->setFlags(nameItem->flags() ^ Qt::ItemIsEditable);
        QTableWidgetItem *sizeItem = new QTableWidgetItem(QString("%1 Kb").arg(size / BYTES_IN_KB + 1));
        sizeItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        sizeItem->setFlags(sizeItem->flags() ^ Qt::ItemIsEditable);

        int row = ui->filesTable->rowCount();
        ui->filesTable->insertRow(row);
        ui->filesTable->setItem(row, 0, nameItem);
        ui->filesTable->setItem(row, 1, sizeItem);
    }
}

void FindWindow::slotOpenFile(int row, int)
{
    QTableWidgetItem *item = ui->filesTable->item(row, 0);
    QDesktopServices::openUrl(QUrl::fromLocalFile(ui->dirComboBox->currentText() + item->text()));
}

FindWindow::~FindWindow()
{
    delete ui;
}
