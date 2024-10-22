#include "commandreceiver.h"
#include <QDebug>

CommandReceiver::CommandReceiver(QObject *parent) : QObject(parent), socket(new QUdpSocket(this))
{
    connect(socket, &QUdpSocket::readyRead, this, &CommandReceiver::onReadyRead);
}

void CommandReceiver::bindSocket(const QHostAddress &address, quint16 port)
{
    bool bindResult = socket->bind(address, port);
    if (!bindResult) {
        qDebug() << "Ошибка привязки к порту";
    }
}

QUdpSocket* CommandReceiver::getSocket() const {
    return socket;
}


void CommandReceiver::onReadyRead(){
    while (socket->hasPendingDatagrams()) {
        QByteArray buffer;
        buffer.resize(socket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;
        socket->readDatagram(buffer.data(), buffer.size(), &sender, &senderPort);

        QString command = QString::fromUtf8(buffer);

        qDebug() << "Получено сообщение от:" << sender.toString() << ":" << senderPort << command;

        handleCommand(command);
    }
}

void CommandReceiver::handleCommand(const QString &command)
{
    if (command == "startCamera") {
        qDebug() << "Запуск камеры.";
        emit startCameraCommand();
    } else if (command == "stopCamera") {
        qDebug() << "Остановка камеры.";
        emit stopCameraCommand();
    } else if (command == "startDisplay") {
        qDebug() << "Запуск отображения.";
        emit startDisplayCommand();
    } else if (command == "stopDisplay") {
        qDebug() << "Остановка отображения.";
        emit stopDisplayCommand();
    } else {
        qDebug() << "Неизвестная команда: " << command;
    }
}
