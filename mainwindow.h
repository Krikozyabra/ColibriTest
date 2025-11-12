#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

private:
    Ui::MainWindow *ui;

    void setupToolButton();
};
#endif // MAINWINDOW_H
