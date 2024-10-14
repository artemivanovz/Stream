/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
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
        cameraViewfinder = new QCameraViewfinder(centralwidget);
        cameraViewfinder->setObjectName(QString::fromUtf8("cameraViewfinder"));
        cameraViewfinder->setGeometry(QRect(110, 10, 531, 361));
        StreamButton = new QPushButton(centralwidget);
        StreamButton->setObjectName(QString::fromUtf8("StreamButton"));
        StreamButton->setGeometry(QRect(390, 380, 231, 131));
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
        CameraButton->setText(QCoreApplication::translate("MainWindow", "\320\232\320\220\320\234\320\225\320\240\320\220 \320\222\320\232\320\233", nullptr));
        StreamButton->setText(QCoreApplication::translate("MainWindow", "\320\242\320\240\320\220\320\235\320\241\320\233\320\257\320\246\320\230\320\257 \320\222\320\232\320\233", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
