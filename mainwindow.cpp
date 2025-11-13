#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QMenu>
#include "./fileprogress.h"
#include "config.h"
#include "tool.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::startProcess);
    emit ui->timerCheck->checkStateChanged(Qt::Unchecked);

    setupToolButton();
}

// Функция для настройки меню в ToolButton
void MainWindow::setupToolButton(){
    QMenu *menu = new QMenu();

    QAction *defaultAction = new QAction("Rewrite", this);
    menu->addAction(defaultAction);

    QAction *countingAction = new QAction("Modify", this);
    menu->addAction(countingAction);

    ui->reapitingChoose->setMenu(menu);
    ui->reapitingChoose->triggered(defaultAction);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Функция обрабатывает эвент clicked у кнопки и вызывает диалоговое окно, в котором пользователь выбирает папку с файлами
void MainWindow::on_inputFolderSelectButton_clicked()
{
    QString folderPath = QFileDialog::getExistingDirectory(this,
                                                         tr("Open Folder"),
                                                         QDir::homePath(),
                                                         QFileDialog::ShowDirsOnly);
    if(!folderPath.isEmpty()){
        ui->inputFolerPathLine->setText(folderPath);
    }else{
        QMessageBox::warning(this, tr("File Path is empty!"), tr("You didn't select a folder."));
    }
}

// Триггер на ивент измения в ToolButton
void MainWindow::on_reapitingChoose_triggered(QAction *arg1)
{
    ui->reapitingChoose->setText(arg1->text());
}

// Функция обрабатывает эвент clicked у кнопки и вызывает диалоговое окно, в котором пользователь выбирает папку с файлами
void MainWindow::on_outputFolderSelectButton_clicked()
{
    QString folderPath = QFileDialog::getExistingDirectory(this,
                                                           tr("Open Folder"),
                                                           QDir::homePath(),
                                                           QFileDialog::ShowDirsOnly);
    if(!folderPath.isEmpty()){
        ui->outputFolderPathLine->setText(folderPath);
    }else{
        QMessageBox::warning(this, tr("File Path is empty!"), tr("You didn't select a folder."));
    }
}

void MainWindow::on_startButton_clicked()
{
    Config *c = Config::GetInstace();
    ui->startButton->setEnabled(false);
    ui->stopButton->setEnabled(true);
    if(!c->getWithTimer()){
        emit this->startProcess();
    }else{
        emit this->startProcess();
        c->setTimerLength(ui->timerLengthLine->text().toFloat());
        this->timer->start(c->getTimerLength()*1000);
        emit this->enablingInterface(false);
    }
}

void MainWindow::on_stopButton_clicked()
{
    this->timer->stop();
    emit this->enablingInterface(true);
    ui->startButton->setEnabled(true);
    ui->stopButton->setEnabled(false);
}

void MainWindow::handleAllFilesModified(){
    ui->startButton->setEnabled(true);
    ui->stopButton->setEnabled(false);
}


void MainWindow::enablingInterface(bool flag){
    ui->timerLengthLine->setEnabled(flag);
    ui->fileMaskLine->setEnabled(flag);
    ui->inputFolerPathLine->setEnabled(flag);
    ui->inputFolderSelectButton->setEnabled(flag);
    ui->xorDataLine->setEnabled(flag);
    ui->timerCheck->setEnabled(flag);
    ui->reapitingChoose->setEnabled(flag);
    ui->outputFolderPathLine->setEnabled(flag);
    ui->outputFolderSelectButton->setEnabled(flag);
    ui->deleteInputCheck->setEnabled(flag);
}

void MainWindow::startProcess(){
    Config *config = Config::GetInstace();
    if(config->getNumberOfFiles() > 0){
        QString m = "На данный момент обрабатывается " +
                    QString::number(config->getNumberOfFiles()) +
                    " файлов, поэтому дождитесь завершения";
        ui->infoLabel->setText(m);
        return;
    }

    ui->infoLabel->setText("");
    // Запись 8-байтовой переменной
    config->setKeyCode(ui->xorDataLine->text().toStdString());
    // Получаем настройку на удаление входных файлов по окончании работы
    config->setDeletingInputFilesFlag(ui->deleteInputCheck->checkState());
    // Получаем настройку повторяющихся файлов
    config->setOutputFilesMode((ui->reapitingChoose->text().contains("Modify") ? MODIFY_FILES : REWRITE_FILES));

    // Проверка на существование выбранной входной директории
    QDir inputFolderPath(ui->inputFolerPathLine->text());
    if(!inputFolderPath.exists()){
        QMessageBox::warning(this, tr("Broken directory"), tr("Selected input directory is not exist."));
        return;
    }

    // Проверка на существование выбранной выходящей директории
    QDir outputFolderPath(ui->outputFolderPathLine->text());
    if(!outputFolderPath.exists()){
        QMessageBox::warning(this, tr("Broken directory"), tr("Selected output directory is not exist."));
        return;
    }

    // Создание списка фильтров для файлов
    QString fileMasks = ui->fileMaskLine->text();
    if(fileMasks.isEmpty()) {
        QMessageBox::warning(this, tr("No file mask"), tr("You didn't type file mask"));
        return;
    }
    QStringList fileMasksList;

    const auto fileMasksSplitted = fileMasks.split(',');
    for(const QString &fileMask : fileMasksSplitted){
        fileMasksList << fileMask.trimmed();
    }

    // Поиск файлов в указанной папке с указанынми фильтрами
    const auto files = inputFolderPath.entryInfoList(fileMasksList, QDir::Files);
    if(files.isEmpty()){
        ui->infoLabel->setText("No files found");
        this->handleAllFilesModified();
        return;
    }

    // Получение vertical layout, в котором будет размещатсья информация об обрабатываемых файлах
    QVBoxLayout *fileInfoLayout = ui->filesList;
    // Очистка layout, если уже был использован
    if(!fileInfoLayout->isEmpty()){
        QLayoutItem *child;
        while ((child = fileInfoLayout->takeAt(0)) != 0) {
            // Check if the item has a widget and delete it
            if (child->widget()) {
                delete child->widget();
            }
            // Delete the layout item itself
            delete child;
        }
    }

    QFileInfo outputFile;
    for(const QFileInfo &inputFile : files){
        if(config->getOutputFilesMode() == MODIFY_FILES){
            outputFile = defineOutputFile(inputFile, outputFolderPath);
        }else if(config->getOutputFilesMode() == REWRITE_FILES){
            outputFile = createNewOutputFile(inputFile, outputFolderPath);
        }
        FileProgress *fileWidget = new FileProgress(inputFile,
                                                    outputFile,
                                                    this);
        connect(fileWidget, &FileProgress::allFilesModified, this, &MainWindow::handleAllFilesModified);
        fileInfoLayout->addWidget(fileWidget);
        config->addFileInProcess();
    }
}

void MainWindow::on_timerCheck_checkStateChanged(const Qt::CheckState &arg1)
{
    Config *c = Config::GetInstace();
    ui->timerLengthLine->setVisible(arg1);
    ui->stopButton->setVisible(arg1);
    c->setWithTime(arg1);
}



