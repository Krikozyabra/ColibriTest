#ifndef TOOL_H
#define TOOL_H

#include <QString>
#include "fileprogress.h"

void xoring(QString inputPath, QString outputPath, FileProgress *fp);
QFileInfo defineOutputFile(QFileInfo inputFile, QDir outputFolderPath);
QFileInfo createNewOutputFile(QFileInfo inputFile, QDir outputFolderPath);

#endif // TOOL_H
