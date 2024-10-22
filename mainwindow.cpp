#include "mainwindow.h"
#include "imagesender.h"
#include "commandreceiver.h"
#include "./ui_mainwindow.h"
#include <QCamera>
#include <QCameraViewfinder>
#include <QBuffer>
#include <QDebug>
#include <QTimer>
#include <QSettings>
#include <QFile>

QString MainWindow::targetAddress = "";
quint16 MainWindow::targetPort = 0;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , camera(nullptr)
    , imageCapture(nullptr)
    , imageSender(new imagesender(this))
    , configManager(new ConfigManager(this))
    , commandReceiver(new CommandReceiver(this))
    , capturing(true)
    , frameNumber(0)
{
    ui->setupUi(this);

    camera = new QCamera(this);
    camera->setViewfinder(ui->cameraViewfinder);

    targetAddress = configManager->getTargetAddress();
    targetPort = configManager->getTargetPort();

    imageCapture = new QCameraImageCapture(camera,this);
    connect(imageCapture, &QCameraImageCapture::imageCaptured,this, &MainWindow::captureImage);

    connect(commandReceiver, &CommandReceiver::startCameraCommand, this, &MainWindow::startCamera);
    connect(commandReceiver, &CommandReceiver::stopCameraCommand, this, &MainWindow::stopCamera);
    connect(commandReceiver, &CommandReceiver::startDisplayCommand, this, &MainWindow::startDisplay);
    connect(commandReceiver, &CommandReceiver::stopDisplayCommand, this, &MainWindow::stopDisplay);

    ui->CameraButton->setCheckable(true);
    ui->CameraButton->setChecked(false);

    ui->StreamButton->setCheckable(true);
    ui->StreamButton->setChecked(false);

    commandReceiver->bindSocket(QHostAddress::LocalHost, 12345);

}

MainWindow::~MainWindow()
{
    delete ui;
    socket->close();
}

void MainWindow::on_CameraButton_clicked(bool checked)
{
    if(checked){
        startCamera();
    }
    else{
        stopCamera();
    }
}

void MainWindow::on_StreamButton_clicked(bool checked)
{
    if (checked) {
        startDisplay();
    } else {
        stopDisplay();
    }
}

void MainWindow::captureImage(int id,const QImage &image){
    Q_UNUSED(id);

    QUdpSocket *commandSocket = commandReceiver->getSocket();

    QImage greyImage = image.convertToFormat(QImage::Format_Grayscale8);

    imageSender->sendImage(greyImage, QHostAddress(targetAddress), targetPort, frameNumber, commandSocket);
    frameNumber++;

    if (capturing) {
          QMetaObject::invokeMethod(imageCapture, "capture", Qt::QueuedConnection);
    }
}


void MainWindow::startCamera(){
    ui->CameraButton->setText("КАМЕРА ВЫКЛ");
    ui->cameraViewfinder->show();
    camera->start();
    ui->CameraButton->setChecked(true);
    qDebug() << "Команда startCamera выполнена";
}
void MainWindow::stopCamera(){
    ui->CameraButton->setText("КАМЕРА ВКЛ");
    ui->cameraViewfinder->hide();
    camera->stop();
    ui->CameraButton->setChecked(false);
    qDebug() << "Команда stopCamera выполнена";
}
void MainWindow::startDisplay(){
    ui->StreamButton->setText("ТРАНСЛЯЦИЯ ВЫКЛ");
    capturing = true;
    imageCapture->capture();
    ui->StreamButton->setChecked(true);
    qDebug() << "Команда startDisplay выполнена";
}
void MainWindow::stopDisplay(){
    ui->StreamButton->setText("ТРАНСЛЯЦИЯ ВКЛ");
    capturing = false;
    frameNumber =0;
    ui->StreamButton->setChecked(false);
    qDebug() << "Команда stopDisplay выполнена";
}

