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

    // Конвертируем изображение в формат QByteArray
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);
    if (!image.save(&buffer, "JPEG")) {
        qDebug() << "Ошибка: не удалось сохранить изображение.";
        return;
    }

    // Отправляем данные
    sendMessage(byteArray, QHostAddress(targetAddress), targetPort);
    qDebug() << "Изображение отправлено";
    if (capturing) {
        qDebug() << "Новая отправка с задержкой";
        QTimer::singleShot(50, this, [this]() {
            imageCapture->capture();
        });
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

        stream << static_cast<quint16>(frameNumber);  // Номер кадра (например, всегда 0 для статического изображения)
        stream << static_cast<quint16>(totalRows);  // Общее количество строк в кадре
        stream << static_cast<quint16>(i);  // Номер текущей строки
        stream << static_cast<quint16>(frame.bytesPerLine());  // Размер строки в байтах
        stream << static_cast<quint8>(bytesPerPixel);  // Количество байт на пиксель
        stream << static_cast<quint8>(0);  // Резервное поле

        qDebug() << "ВСЕГО СТРОК" << totalRows;
        qDebug() << "РАЗМЕР СТРОКИ" << frame.bytesPerLine();
        qDebug() << "КОЛИЧЕСТВО БАЙТ НА ПИКСЕЛЬ" << bytesPerPixel;
        qDebug() << "НОМЕР СТРОКИ" << i;

        QByteArray rowData(reinterpret_cast<const char*>(frame.scanLine(i)), frame.bytesPerLine());

        // Добавляем данные строки в пакет
        stream.writeRawData(rowData.constData(), rowData.size());

        // Отправка пакета
        qDebug() << "пакет успешно отпрален. Размер:" << packet.size();
        socket->writeDatagram(packet, QHostAddress(targetAddress), targetPort);

        QEventLoop loop;
        QTimer::singleShot(5, &loop, &QEventLoop::quit);
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

        Command cmd = stringToCommand(command);

        qDebug() << cmd;

        switch (cmd) {
        case CAMERA_ON:
            startCamera();
            break;
        case CAMERA_OFF:
            stopCamera();
            break;
        case DISPLAY_ON:
            startDisplay();
            break;
        case DISPLAY_OFF:
            stopDisplay();
            break;
        case UNKNOWN:
        default:
            qDebug() << "Неизвестная команда: " << command;
            break;
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

MainWindow::Command MainWindow::stringToCommand(const QString &command) {
    if (command == "startCamera") {
        return CAMERA_ON;
    } else if (command == "stopCamera") {
        return CAMERA_OFF;
    } else if (command == "startDisplay") {
        return DISPLAY_ON;
    } else if (command == "stopDisplay") {
        return DISPLAY_OFF;
    } else {
        return UNKNOWN;
    }
}

