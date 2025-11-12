#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Функция обрабатывает эвент clicked у кнопки и считывает путь до папки и маску файлов и все найденные файли выписывает в label
void MainWindow::on_scanButton_clicked()
{
    // Проверка на существование выбранной директории
    QDir folderPath(ui->folerPathLine->text());
    if(!folderPath.exists()){
        QMessageBox::warning(this, tr("Broken directory"), tr("Selected directory is not exist."));
    }

    // Создание списка фильтров для файлов
    QString fileMasks = ui->fileMaskEdit->text();
    if(fileMasks.isEmpty()) {
        QMessageBox::warning(this, tr("No file mask"), tr("You didn't type file mask"));
        return;
    }
    QStringList fileMasksList;
    for(auto fileMask : fileMasks.split(',')){
        fileMasksList << fileMask.trimmed();
    }

    // Поиск файлов в указанной папке с указанынми фильтрами
    QFileInfoList files = folderPath.entryInfoList(fileMasksList, QDir::Files);
    if(files.isEmpty()){
        ui->fileList->setText("No files found");
        return;
    }

    //Отображение файлов
    ui->fileList->setText("List of founded files:");
    for(const auto &file : files){
        ui->fileList->setText(ui->fileList->text() + '\n' + file.fileName());
    }
}

// Функция обрабатывает эвент clicked у кнопки и вызывает диалоговое окно, в котором пользователь выбирает папку с файлами
void MainWindow::on_folderSelectButton_clicked()
{
    QString folderPath = QFileDialog::getExistingDirectory(this,
                                                         tr("Open Folder"),
                                                         QDir::homePath(),
                                                         QFileDialog::ShowDirsOnly);
    if(!folderPath.isEmpty()){
        ui->folerPathLine->setText(folderPath);
    }else{
        QMessageBox::warning(this, tr("File Path is empty!"), tr("You didn't select a folder."));
    }
}

