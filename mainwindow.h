#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_inputFolderSelectButton_clicked();

    void on_reapitingChoose_triggered(QAction *arg1);

    void on_outputFolderSelectButton_clicked();

    void on_startButton_clicked();

    void on_timerCheck_checkStateChanged(const Qt::CheckState &arg1);

    void startProcess();

    void enablingInterface(bool flag);

    void on_stopButton_clicked();

private:
    Ui::MainWindow *ui;
    QTimer *timer;
    void setupToolButton();
};
#endif // MAINWINDOW_H
