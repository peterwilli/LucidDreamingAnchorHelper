#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <math.h>
#include <QDir>
#include <QMessageBox>
#include <dreamimagelabel.h>
#include <QApplication>
#include <QDesktopWidget>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    this->supportedExtensions = new QStringList {
            "*.jpg", "*.JPG", "*.png", "*.PNG", "*.JPEG", "*.jpeg", "*.gif", "*.GIF"
    };
    this->imagesDir = new QString(QDir::current().filePath("AnchorHelper_Images"));
    this->initImagesFolder();
    ui->setupUi(this);
    ui->introLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
    ui->introLabel->setOpenExternalLinks(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initImagesFolder() {
    if(!QDir(*this->imagesDir).exists()) {
        QDir().mkdir(*this->imagesDir);
    }
}

bool MainWindow::imagesInFolderCheck() {
    QStringList images = QDir(*this->imagesDir).entryList(*supportedExtensions, QDir::Files);
    if(images.length() == 0) {
        QMessageBox messageBox;
        messageBox.critical(0, "Error", QString("You need to place at least 1 image in %1!").arg(*this->imagesDir));
        messageBox.setFixedSize(500, 200);
    }
    return images.length() == 0;
}

void MainWindow::setDreamTriggerTimer() {
    uint16_t positionMinutes = ui->timerDial->value() * ceil(120 / ui->timerDial->maximum());
    // Add random deviation
    positionMinutes += rand() % ui->deviationBox->value();
    this->timer->setSingleShot(true);
    this->timer->start(positionMinutes * 1000 * 60);
}

void MainWindow::tick() {
    this->openRandomWindow();
    this->setDreamTriggerTimer();
}

void MainWindow::openRandomWindow() {
    if(imagesInFolderCheck()) {
        return;
    }
    if(this->randomWindow != NULL) {
        return;
    }
    uint16_t windowWidth = 320;
    uint8_t windowHeight = 240;
    QRect screenRes = QApplication::desktop()->screenGeometry();
    this->randomWindow = new QWidget();
    this->randomWindow->setAttribute(Qt::WA_ShowWithoutActivating);
    this->randomWindow->resize(windowWidth, windowHeight);
    this->randomWindow->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
    this->randomWindow->move(rand() % (screenRes.width() - windowWidth), rand() % (screenRes.height() - windowHeight));
    QStringList images = QDir(*this->imagesDir).entryList(*supportedExtensions, QDir::Files);
    uint16_t imageNumber = rand() % images.length();
    printf("Selected image: %s\n", images.at(imageNumber).toStdString().c_str());
    QPixmap pm(QDir(*this->imagesDir).filePath(images.at(imageNumber)));
    DreamImageLabel *windowLabel = new DreamImageLabel(this->randomWindow);
    connect(windowLabel, SIGNAL(clicked()), this, SLOT(dreamLabelClick()));

    windowLabel->setPixmap(pm);
    windowLabel->setScaledContents(true);
    windowLabel->move(0, 0);
    windowLabel->resize(windowWidth, windowHeight);

    this->randomWindow->show();
    QTimer::singleShot(60000, std::bind(&MainWindow::closeRandomWindow, this));
}

void MainWindow::dreamLabelClick() {
     this->closeRandomWindow();
}

void MainWindow::closeRandomWindow() {
    if(this->randomWindow == NULL) {
        return;
    }
    delete this->randomWindow;
    this->randomWindow = NULL;
}

void MainWindow::on_startBtn_clicked()
{
    if(imagesInFolderCheck()) {
        return;
    }
    if(this->timer != NULL) {
        delete this->timer;
        this->timer = NULL;
    }

    this->timer = new QTimer(this);
    connect(this->timer, &QTimer::timeout, this, QOverload<>::of(&MainWindow::tick));
    this->setDreamTriggerTimer();
    this->ui->startBtn->setEnabled(false);
}

void MainWindow::on_timerDial_sliderMoved(int position)
{
    uint8_t positionMinutes = position * ceil(120 / ui->timerDial->maximum());
    QString newText = QString("<html><head/><body><p>Currently, it is set to randomly appear each <span style=\"font-weight:600;\">%1 minutes!</span></p></body></html>").arg(positionMinutes);
    ui->durationLabel->setText(newText);
}

void MainWindow::on_pushButton_clicked()
{
    openRandomWindow();
}
