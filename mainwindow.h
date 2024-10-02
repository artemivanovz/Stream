#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QCamera>
#include <QCameraViewfinder>
#include <QCameraImageCapture>
#include <QMainWindow>
#include <QUdpSocket>
#include <QHostAddress>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void sendMessage();
    enum Command {
        CAMERA_ON,
        CAMERA_OFF,
        DISPLAY_ON,
        DISPLAY_OFF,
        UNKNOWN
    };
private slots:

    void on_pushButton_clicked(bool checked);
    void readPendingDatagrams();
    void captureImage(int id, const QImage &image);
    // void captureAndSendImage();
    void startCamera();
    void stopCamera();
    void startDisplay();
    void stopDisplay();
    void sendMessage(const QByteArray &data, const QHostAddress &address, quint16 port);
    void on_pushButton_2_clicked(bool checked);
    // void processCommand(const QString &command);

private:
    Ui::MainWindow *ui;
    QCamera *camera;
    QUdpSocket *socket;
    QTimer *captureTimer;
    QCameraImageCapture *imageCapture;
    QString targetAddress;
    quint16 targetPort;
    Command stringToCommand(const QString &command);
    QTimer *packetTimer;
    int totalRows;      // Общее количество строк
    int bytesPerPixel;
    int currentRow;  // Переменная для отслеживания текущей строки
    QImage frame;    // Изображение, которое будет передаваться
    bool capturing;
    int frameNumber;
};
#endif // MAINWINDOW_H
