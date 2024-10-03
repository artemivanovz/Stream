#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QCamera>
#include <QCameraViewfinder>
#include <QVBoxLayout>
#include <QBuffer>
#include <QDebug>
#include <QTimer>
#include <QThread>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , camera(nullptr)
    , targetAddress("127.0.0.1")
    , targetPort(12346)
    , capturing(true)
    , frameNumber(0)
{
    ui->setupUi(this);

    camera = new QCamera(this);
    camera->setViewfinder(ui->cameraViewfinder);

    imageCapture = new QCameraImageCapture(camera,this);
    connect(imageCapture, &QCameraImageCapture::imageCaptured,this, &MainWindow::captureImage);

    ui->pushButton->setCheckable(true);
    ui->pushButton->setChecked(false);

    ui->pushButton_2->setCheckable(true);
    ui->pushButton_2->setChecked(false);

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

void MainWindow::on_pushButton_clicked(bool checked)
{
    if(checked){
        ui->pushButton->setText("КАМЕРА ВЫКЛ");
        ui->cameraViewfinder->show();
        camera->start();
    }
    else{
        ui->pushButton->setText("КАМЕРА ВКЛ");
        ui->cameraViewfinder->hide();
        camera->stop();
    }
}

void MainWindow::captureImage(int id,const QImage &image){
    Q_UNUSED(id);

    QImage grayImage = image.convertToFormat(QImage::Format_Grayscale8);
    // Конвертируем изображение в формат QByteArray
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);
    if (!grayImage.save(&buffer, "JPEG")) {
        qDebug() << "Ошибка: не удалось сохранить изображение.";
        return;
    }

    // Отправляем данные
    sendMessage(byteArray, QHostAddress(targetAddress), targetPort);
    if (capturing) {
        // QTimer::singleShot(50, this, [this]() {
            imageCapture->capture();
        // });
    }
}

void MainWindow::sendMessage(const QByteArray &data, const QHostAddress &address, quint16 port)
{
    frameNumber++;
    QImage frame;
    if(!frame.loadFromData(data,"JPEG")){
        qDebug() << "Ошибка: не удалось сохранить изображение.";
    }
    int totalRows = frame.height();
    int bytesPerPixel = frame.depth() / 8;
    for(int i = 0 ; i < totalRows; ++i){
        QByteArray packet;
        QDataStream stream(&packet, QIODevice::WriteOnly);

        stream << static_cast<quint16>(frameNumber);
        stream << static_cast<quint16>(totalRows);
        stream << static_cast<quint16>(i);
        stream << static_cast<quint16>(frame.bytesPerLine());
        stream << static_cast<quint8>(bytesPerPixel);
        stream << static_cast<quint8>(0);

        QByteArray rowData(reinterpret_cast<const char*>(frame.scanLine(i)), frame.bytesPerLine());

        // Добавляем данные строки в пакет
        stream.writeRawData(rowData.constData(), rowData.size());

        // Отправка пакета
        socket->writeDatagram(packet, QHostAddress(targetAddress), targetPort);

        QEventLoop loop;
        QTimer::singleShot(1, &loop, &QEventLoop::quit);
        loop.exec();
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

void MainWindow::on_pushButton_2_clicked(bool checked)
{
    if (checked) {
        ui->pushButton_2->setText("ТРАНСЛЯЦИЯ ВЫКЛ");
        capturing = true;
        imageCapture->capture();
    } else {
        ui->pushButton_2->setText("ТРАНСЛЯЦИЯ ВКЛ");
        capturing = false;
    }
}

void MainWindow::startCamera(){
    ui->pushButton->setText("КАМЕРА ВЫКЛ");
    ui->cameraViewfinder->show();
    camera->start();
    ui->pushButton->setChecked(true);
    qDebug() << "Команда startCamera выполнена";
}
void MainWindow::stopCamera(){
    ui->pushButton->setText("КАМЕРА ВКЛ");
    ui->cameraViewfinder->hide();
    camera->stop();
    ui->pushButton->setChecked(false);
    qDebug() << "Команда stopCamera выполнена";
}
void MainWindow::startDisplay(){
    ui->pushButton_2->setText("ТРАНСЛЯЦИЯ ВЫКЛ");
    capturing = true;
    imageCapture->capture();
    ui->pushButton_2->setChecked(true);
    qDebug() << "Команда startDisplay выполнена";
}
void MainWindow::stopDisplay(){
    ui->pushButton_2->setText("ТРАНСЛЯЦИЯ ВКЛ");
    capturing = false;
    frameNumber =0;
    ui->pushButton_2->setChecked(false);
    qDebug() << "Команда stopDisplay выполнена";
}

