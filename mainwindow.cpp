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
#include <QSizePolicy>

QString MainWindow::targetAddress = "";
quint16 MainWindow::targetPort = 0;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , camera(nullptr)
    , imageSender(new imagesender(this))
    , configManager(new ConfigManager(this))
    , commandReceiver(new CommandReceiver(this))
    , videoProbe(nullptr)
    , capturing(false)
    , frameNumber(0)
{
    ui->setupUi(this);

    camera = new QCamera(this);

    videoProbe = new QVideoProbe(this);
    if (videoProbe->setSource(camera)) {
        connect(videoProbe, &QVideoProbe::videoFrameProbed, this, &MainWindow::processFrame);
    } else {
        ui->imageLabel->setText("Не удалось подключить QVideoProbe");
        return;
    }

    QSizePolicy sp_retain = ui->imageLabel->sizePolicy();
    sp_retain.setRetainSizeWhenHidden(true);
    ui->imageLabel->setSizePolicy(sp_retain);

    targetAddress = configManager->getTargetAddress();
    targetPort = configManager->getTargetPort();

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

void MainWindow::processFrame(const QVideoFrame &frame)
{

    QVideoFrame cloneFrame(frame);
    if (!cloneFrame.map(QAbstractVideoBuffer::ReadOnly)) {
        qWarning() << "Не удалось отобразить кадр";
        return;
    }

    QImage image;
    image = cloneFrame.image().convertToFormat(QImage::Format_RGB32);

    cloneFrame.unmap();

    if (image.isNull()) {
        qWarning() << "Изображение пустое";
        return;
    }

    // Отображение изображения на QLabel
    ui->imageLabel->setPixmap(QPixmap::fromImage(image).scaled(ui->imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

    if (capturing) {
        QMetaObject::invokeMethod(this, "sendImage", Qt::QueuedConnection, Q_ARG(QImage, image));
    }

}

void MainWindow::sendImage(const QImage &image)
{
    QImage greyImage = image.convertToFormat(QImage::Format_Grayscale8);

    imageSender->sendImage(greyImage, QHostAddress(targetAddress), targetPort, frameNumber, commandReceiver->getSocket());
    frameNumber++;
}

void MainWindow::sendCommand(const QString &command){
    QUdpSocket *commandSocket = commandReceiver->getSocket();
    QByteArray data = command.toUtf8();
    QByteArray packet;
    QDataStream stream(&packet, QIODevice::WriteOnly);
    stream << QString("CMD") << QString(data);
    commandSocket->writeDatagram(packet,QHostAddress(targetAddress),targetPort);
}

void MainWindow::startCamera(){
    ui->CameraButton->setText("КАМЕРА ВЫКЛ");
    sendCommand("startCamera");
    ui->imageLabel->show();
    camera->start();
    ui->CameraButton->setChecked(true);
    qDebug() << "Команда startCamera выполнена";
}

void MainWindow::stopCamera(){
    ui->CameraButton->setText("КАМЕРА ВКЛ");
    sendCommand("stopCamera");
    ui->imageLabel->hide();
    camera->stop();
    ui->CameraButton->setChecked(false);
    qDebug() << "Команда stopCamera выполнена";
}

void MainWindow::startDisplay(){
    ui->StreamButton->setText("ТРАНСЛЯЦИЯ ВЫКЛ");
    sendCommand("startDisplay");
    capturing = true;
    // imageCapture->capture();
    ui->StreamButton->setChecked(true);
    qDebug() << "Команда startDisplay выполнена";
}

void MainWindow::stopDisplay(){
    ui->StreamButton->setText("ТРАНСЛЯЦИЯ ВКЛ");
    sendCommand("stopDisplay");
    capturing = false;
    frameNumber =0;
    ui->StreamButton->setChecked(false);
    qDebug() << "Команда stopDisplay выполнена";
}

