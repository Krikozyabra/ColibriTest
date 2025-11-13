#include "tool.h"
#include "fileprogress.h"
#include <fstream>
#include <cstdio>
#include "config.h"

void xoring(QString inputPath, QString outputPath, FileProgress *fp)
{
    Config *config = Config::GetInstace();
    std::ifstream inputStream(inputPath.toStdString() ,std::ios::binary);
    std::ofstream outputStream(outputPath.toStdString(), std::ios::binary);
    if(!inputStream.is_open()){
        emit fp->hideProgressBar(true);
        emit fp->showError(inputPath + " <-can't be readed");
        inputStream.close();
        outputStream.close();
        return;
    }

    if(!outputStream.is_open()){
        emit fp->hideProgressBar(true);
        emit fp->showError(outputPath + " <-can't be writed");
        inputStream.close();
        outputStream.close();
        return;
    }

    size_t i = 0;
    char buffer;
    std::string key = config->getKeyCode();
    int keyLen = config->getKeyCodeLen();
    while(inputStream.get(buffer)){
        buffer ^= key[i%keyLen];
        outputStream.put(buffer);
        if (i % 1024 == 0)  // Делаем обнолвение не каждый тиак, чтобы уменьшить лаги
            emit fp->progressChanged(i);
        ++i;
    }

    config->delFileInProcess();

    inputStream.close();
    // Если флаг на удлаение входных файлов установлен, то удаляем файл
    if(config->getDeletingInputFilesFlag()) std::remove(inputPath.toStdString().c_str());
    outputStream.close();

    emit fp->progressChanged(i); // Последнее обновление
}

// Создаем при необходимости выходной файл и возвращаем информацию о нем
QFileInfo defineOutputFile(QFileInfo inputFile, QDir outputFolderPath){
    // Проверяем повторяется ли название выходного файла
    auto outputFilesTest = outputFolderPath.entryInfoList(
        QStringList()<<inputFile.fileName());

    // Если есть повторения
    if(!outputFilesTest.isEmpty()){
        // Делим название выходного файла, чтобы построить новое для будущего
        auto splittedFileName = inputFile.fileName().split(".");
        // Строится новое название файла
        QStringList newFileFilter = QStringList()<<QStringList()<<splittedFileName[0]+"_*."+splittedFileName[1];
        // Проверка на наличие уже модифицированных названий
        auto candidatesForOutputFile = outputFolderPath.entryInfoList(newFileFilter);
        // настройка последнего id
        size_t id = 1;
        id += candidatesForOutputFile.count();

        // составление пути до нового выходного файла
        QString newOutputFilePath = outputFolderPath.absoluteFilePath(splittedFileName[0]+"_"+QString::number(id)+"."+splittedFileName[1]);

        // создание нового файла
        std::ofstream newOF(newOutputFilePath.toStdString());
        newOF.close();

        // возвращение информации о новом файле
        const auto res = outputFolderPath.entryInfoList(newFileFilter).constLast();
        return res;
    }else{
        return outputFilesTest.last();
    }
}

QFileInfo createNewOutputFile(QFileInfo inputFile, QDir outputFolderPath)
{
    std::ofstream outputFile(outputFolderPath.absoluteFilePath(inputFile.fileName()).toStdString());
    outputFile.close();

    const auto res = outputFolderPath.entryInfoList(QStringList()<<inputFile.fileName()).constLast();
    return res;
}
