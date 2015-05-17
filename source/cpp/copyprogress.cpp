#include "copyprogress.h"
#include "ui_copyprogress.h"

CopyProgress::CopyProgress(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CopyProgress)
{
    ui->setupUi(this);
    this->setWindowTitle("Copying files...");
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::Tool);
    ui->totalProgressBar->setValue(0);
    ui->fileProgressBar->setValue(0);
    connect(ui->abortButton, SIGNAL(clicked()), this, SLOT(slotAbortCopy()));
}

void CopyProgress::slotAbortCopy()
{
    emit abortCopy();
}

CopyProgress::~CopyProgress()
{
    delete ui;
}

void CopyProgress::slotSetMaxTotal(quint64 maxTotal)
{
    ui->totalProgressBar->setMaximum(maxTotal);
}

void CopyProgress::slotSetCurrentTotal(quint64 currentTotal)
{
    ui->totalProgressBar->setValue(currentTotal);
}

void CopyProgress::slotSetFromTo(QString from, QString to)
{
    ui->fromPathLabel->setText(from);
    ui->toPathLabel->setText(to);
}

void CopyProgress::slotSetMaxSize(quint64 maxSize, quint64 ratio)
{
    this->ratio = ratio;
    ui->fileProgressBar->setMaximum(maxSize / ratio);
}

void CopyProgress::slotSetCurrentSize(quint64 currentSize)
{
    ui->fileProgressBar->setValue(currentSize / ratio);
}
