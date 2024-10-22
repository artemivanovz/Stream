#ifndef COMMANDRECEIVER_H
#define COMMANDRECEIVER_H
#include <QObject>
#include <QUdpSocket>
#include <QHostAddress>

class CommandReceiver : public QObject
{
    Q_OBJECT
public:
    explicit CommandReceiver(QObject *parent = nullptr);
    void bindSocket(const QHostAddress &address, quint16 port);
    void onReadyRead();
    QUdpSocket* getSocket() const;

signals:
    void startCameraCommand();
    void stopCameraCommand();
    void startDisplayCommand();
    void stopDisplayCommand();


private:
    QUdpSocket *socket;
    void handleCommand(const QString &command);  // Метод для обработки команд
};

#endif
