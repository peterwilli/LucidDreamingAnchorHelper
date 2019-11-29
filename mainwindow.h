#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QString>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_startBtn_clicked();
    void on_timerDial_sliderMoved(int position);
    void dreamLabelClick();

    void on_pushButton_clicked();

private:
    void tick();
    void openRandomWindow();
    void closeRandomWindow();
    void initImagesFolder();
    bool imagesInFolderCheck();
    QTimer *timer = NULL;
    QWidget *randomWindow = NULL;
    QString *imagesDir;
    QStringList *supportedExtensions;

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
