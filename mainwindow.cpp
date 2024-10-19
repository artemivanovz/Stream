#include "mainwindow.h"
#include "imagesender.h"
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

    ui->CameraButton->setCheckable(true);
    ui->CameraButton->setChecked(false);

    ui->StreamButton->setCheckable(true);
    ui->StreamButton->setChecked(false);

    socket = new QUdpSocket(this);
    bool bindResult = socket->bind(QHostAddress::LocalHost, 12345);
    if(!bindResult){
        qDebug() << "Ошибка привязки к порту";
    }
    connect(socket, &QUdpSocket::readyRead, this, &MainWindow::readPendingDatagrams);

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

void MainWindow::captureImage(int id,const QImage &image){
    Q_UNUSED(id);

    QImage greyImage = image.convertToFormat(QImage::Format_Grayscale8);

    imageSender->sendImage(greyImage, QHostAddress(targetAddress), targetPort, frameNumber);
    frameNumber++;

    if (capturing) {
          QMetaObject::invokeMethod(imageCapture, "capture", Qt::QueuedConnection);
    }
}

void MainWindow::readPendingDatagrams(){
    while (socket->hasPendingDatagrams()) {
        QByteArray buffer;
        buffer.resize(socket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;
        socket->readDatagram(buffer.data(), buffer.size(), &sender, &senderPort);

        QString command = QString::fromUtf8(buffer);

        qDebug() << "Получено сообщение от:" << sender.toString() << ":" << senderPort << command;

        if (command == "startCamera") {
            startCamera();
        } else if (command == "stopCamera") {
            stopCamera();
        } else if (command == "startDisplay") {
            startDisplay();
        } else if (command == "stopDisplay") {
            stopDisplay();
        } else {
            qDebug() << "Неизвестная команда";
        }

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

