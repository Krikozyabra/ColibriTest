#include "fileprogress.h"
#include "ui_fileprogress.h"
#include "tool.h"
#include "config.h"
#include <thread>

FileProgress::FileProgress(const QFileInfo &inputFile, QFileInfo &outputFile, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FileProgress)
{
    ui->setupUi(this);

    ui->fileNameLabel->setText(inputFile.absoluteFilePath());
    ui->fileProgressBar->setMaximum(inputFile.size());

    connect(this, &FileProgress::progressChanged,
            this, &FileProgress::setProgress, Qt::QueuedConnection);

    connect(this, &FileProgress::showError,
            this, &FileProgress::setLabelText, Qt::QueuedConnection);

    connect(this, &FileProgress::hideProgressBar,
            this, &FileProgress::setProgressBarVisible, Qt::QueuedConnection);

    connect(this, &FileProgress::endFileModify,
            this, &FileProgress::handleEndFileModify, Qt::QueuedConnection);

    xorFile(inputFile, outputFile);
}

FileProgress::~FileProgress()
{
    delete ui;
}

void FileProgress::handleEndFileModify(){
    Config *c = Config::GetInstace();
    c->delFileInProcess();
    if(c->getNumberOfFiles() < 1){
        if(!c->getWithTimer()){
            emit allFilesModified();
        }
    }
}

void FileProgress::setProgress(int currentProgress){
    ui->fileProgressBar->setValue(currentProgress);
}

void FileProgress::setProgressBarVisible(bool flag)
{
    ui->fileProgressBar->setVisible(!flag);
}

void FileProgress::setLabelText(QString text)
{
    ui->fileNameLabel->setText(text);
}

void FileProgress::xorFile(const QFileInfo &inputFile, QFileInfo &outputFile)
{
    std::thread t(xoring, inputFile.absoluteFilePath(), outputFile.absoluteFilePath(), this);
    t.detach();
}
