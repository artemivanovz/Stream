/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.13
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>
#include <qcameraviewfinder.h>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QPushButton *CameraButton;
    QCameraViewfinder *cameraViewfinder;
    QPushButton *StreamButton;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        CameraButton = new QPushButton(centralwidget);
        CameraButton->setObjectName(QString::fromUtf8("CameraButton"));
        CameraButton->setGeometry(QRect(130, 380, 231, 131));
        CameraButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #D2B48C; /* \320\221\320\276\320\273\320\265\320\265 \320\272\320\276\321\200\320\270\321\207\320\275\320\265\320\262\321\213\320\271 \320\276\321\202\321\202\320\265\320\275\320\276\320\272 \320\261\320\265\320\266\320\265\320\262\320\276\320\263\320\276 */\n"
"    border: 2px solid #A0522D; /* \320\221\320\276\320\273\320\265\320\265 \320\272\320\276\320\275\321\202\321\200\320\260\321\201\321\202\320\275\320\260\321\217 \320\276\320\272\320\260\320\275\321\202\320\276\320\262\320\272\320\260 */\n"
"    border-radius: 25px; /* \320\241\320\272\321\200\321\203\320\263\320\273\320\265\320\275\320\275\321\213\320\265 \320\272\321\200\320\260\321\217 */\n"
"    font-family: 'Arial'; /* \320\250\320\270\321\200\320\276\320\272\320\270\320\271 \321\210\321\200\320\270\321\204\321\202 */\n"
"    font-size: 16px; /* \320\240\320\260\320\267\320\274\320\265\321\200 \321\210\321\200\320\270\321\204\321\202\320\260 */\n"
"    font-weight: bold; /* \320\226\320\270\321\200\320\275"
                        "\321\213\320\271 \321\210\321\200\320\270\321\204\321\202 */\n"
"    padding: 10px 20px; /* \320\222\320\275\321\203\321\202\321\200\320\265\320\275\320\275\320\270\320\265 \320\276\321\202\321\201\321\202\321\203\320\277\321\213 */\n"
"    color: #8B4513; /* \320\221\320\276\320\273\320\265\320\265 \320\272\320\276\320\275\321\202\321\200\320\260\321\201\321\202\320\275\321\213\320\271 \321\206\320\262\320\265\321\202 \321\210\321\200\320\270\321\204\321\202\320\260 */\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #C19A6B; /* \320\246\320\262\320\265\321\202 \320\277\321\200\320\270 \320\275\320\260\320\262\320\265\320\264\320\265\320\275\320\270\320\270 */\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: #8B4513; /* \320\246\320\262\320\265\321\202 \320\277\321\200\320\270 \320\275\320\260\320\266\320\260\321\202\320\270\320\270 */\n"
"}"));
        cameraViewfinder = new QCameraViewfinder(centralwidget);
        cameraViewfinder->setObjectName(QString::fromUtf8("cameraViewfinder"));
        cameraViewfinder->setGeometry(QRect(110, 10, 531, 361));
        StreamButton = new QPushButton(centralwidget);
        StreamButton->setObjectName(QString::fromUtf8("StreamButton"));
        StreamButton->setGeometry(QRect(390, 380, 231, 131));
        StreamButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: #D2B48C; /* \320\221\320\276\320\273\320\265\320\265 \320\272\320\276\321\200\320\270\321\207\320\275\320\265\320\262\321\213\320\271 \320\276\321\202\321\202\320\265\320\275\320\276\320\272 \320\261\320\265\320\266\320\265\320\262\320\276\320\263\320\276 */\n"
"    border: 2px solid #A0522D; /* \320\221\320\276\320\273\320\265\320\265 \320\272\320\276\320\275\321\202\321\200\320\260\321\201\321\202\320\275\320\260\321\217 \320\276\320\272\320\260\320\275\321\202\320\276\320\262\320\272\320\260 */\n"
"    border-radius: 25px; /* \320\241\320\272\321\200\321\203\320\263\320\273\320\265\320\275\320\275\321\213\320\265 \320\272\321\200\320\260\321\217 */\n"
"    font-family: 'Arial'; /* \320\250\320\270\321\200\320\276\320\272\320\270\320\271 \321\210\321\200\320\270\321\204\321\202 */\n"
"    font-size: 16px; /* \320\240\320\260\320\267\320\274\320\265\321\200 \321\210\321\200\320\270\321\204\321\202\320\260 */\n"
"    font-weight: bold; /* \320\226\320\270\321\200\320\275"
                        "\321\213\320\271 \321\210\321\200\320\270\321\204\321\202 */\n"
"    padding: 10px 20px; /* \320\222\320\275\321\203\321\202\321\200\320\265\320\275\320\275\320\270\320\265 \320\276\321\202\321\201\321\202\321\203\320\277\321\213 */\n"
"    color: #8B4513; /* \320\221\320\276\320\273\320\265\320\265 \320\272\320\276\320\275\321\202\321\200\320\260\321\201\321\202\320\275\321\213\320\271 \321\206\320\262\320\265\321\202 \321\210\321\200\320\270\321\204\321\202\320\260 */\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    background-color: #C19A6B; /* \320\246\320\262\320\265\321\202 \320\277\321\200\320\270 \320\275\320\260\320\262\320\265\320\264\320\265\320\275\320\270\320\270 */\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: #8B4513; /* \320\246\320\262\320\265\321\202 \320\277\321\200\320\270 \320\275\320\260\320\266\320\260\321\202\320\270\320\270 */\n"
"}"));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 21));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
#if QT_CONFIG(whatsthis)
        CameraButton->setWhatsThis(QString());
#endif // QT_CONFIG(whatsthis)
        CameraButton->setText(QCoreApplication::translate("MainWindow", "\320\232\320\220\320\234\320\225\320\240\320\220 \320\222\320\232\320\233", nullptr));
        StreamButton->setText(QCoreApplication::translate("MainWindow", "\320\242\320\240\320\220\320\235\320\241\320\233\320\257\320\246\320\230\320\257 \320\222\320\232\320\233", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
