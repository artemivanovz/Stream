#ifndef IMAGESENDER_H
#define IMAGESENDER_H

#include <QObject>
#include <QImage>
#include <QUdpSocket>
#include <QHostAddress>

class imagesender : public QObject {
    Q_OBJECT

public:
    imagesender(QObject *parent = nullptr);
    void sendImage(const QImage &image, const QHostAddress &address, quint16 port, int frameNumber);

private:
    QUdpSocket *socket;
};

#endif // IMAGESENDER_H
