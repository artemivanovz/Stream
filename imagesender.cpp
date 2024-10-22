#include "imagesender.h"
#include <QDataStream>
#include <QByteArray>

imagesender::imagesender(QObject *parent) : QObject(parent) {
    socket = new QUdpSocket(this);
}

void imagesender::sendImage(const QImage &image, const QHostAddress &address, quint16 port, int frameNumber,QUdpSocket *hostsocket) {
    int totalRows = image.height();
    int bytesPerPixel = image.depth() / 8;

    for (int i = 0; i < totalRows; ++i) {
        QByteArray packet;
        QDataStream stream(&packet, QIODevice::WriteOnly);

        // Add frame number, total rows, row index, bytes per line, and bytes per pixel to the packet
        stream << static_cast<quint16>(frameNumber);
        stream << static_cast<quint16>(totalRows);
        stream << static_cast<quint16>(i);
        stream << static_cast<quint16>(image.bytesPerLine());
        stream << static_cast<quint8>(bytesPerPixel);
        stream << static_cast<quint8>(0);

        QByteArray rowData(reinterpret_cast<const char*>(image.scanLine(i)), image.bytesPerLine());
        stream.writeRawData(rowData.constData(), rowData.size());

        hostsocket->writeDatagram(packet, address, port);
    }

     frameNumber++;
}
