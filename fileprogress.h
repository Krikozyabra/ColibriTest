#ifndef FILEPROGRESS_H
#define FILEPROGRESS_H

#include <QWidget>
#include <Qdir>

namespace Ui {
class FileProgress;
}

class FileProgress : public QWidget
{
    Q_OBJECT

public:
    explicit FileProgress(const QFileInfo &inputFile, QFileInfo &outputFile, QWidget *parent = nullptr);
    ~FileProgress();

    void setProgress(int currentProgress);
    void setProgressBarVisible(bool flag);
    void setLabelText(QString text);
    void handleEndFileModify();

signals:
    // сигналы необходимы, так как нельзя взаимодействовать напрямую с QPainter из другого потока
    // можно только вызывать сигналы и это по-моему даже круче и лаконичнее
    void progressChanged(int value);
    void showError(QString text);
    void hideProgressBar(bool flag);
    void endFileModify();
    void allFilesModified();

private:
    Ui::FileProgress *ui;

    void xorFile(const QFileInfo &inputFile, QFileInfo &outputFile);
};

#endif // FILEPROGRESS_H
