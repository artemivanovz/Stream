#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "imagesender.h"
#include <QMainWindow>
#include <QCamera>
#include <QCameraViewfinder>
#include <QCameraImageCapture>
#include <QMainWindow>
#include <QUdpSocket>
#include <QHostAddress>

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

private slots:

    void startCamera();
    void stopCamera();
    void startDisplay();
    void stopDisplay();

    void readPendingDatagrams();
    void captureImage(int id, const QImage &image);

    void on_StreamButton_clicked(bool checked);
    void on_CameraButton_clicked(bool checked);

private:
    Ui::MainWindow *ui;
    QCamera *camera;
    QCameraImageCapture *imageCapture;
    imagesender *imageSender;
    QUdpSocket *socket;
    QString targetAddress;
    quint16 targetPort;

    bool capturing;
    int totalRows;
    int bytesPerPixel;
    int currentRow;
    int frameNumber;
};
#endif // MAINWINDOW_H
