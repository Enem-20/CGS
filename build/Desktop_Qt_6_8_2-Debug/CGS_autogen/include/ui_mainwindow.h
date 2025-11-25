/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionParameters_set;
    QAction *actionRefresh_configs;
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QLabel *label_7;
    QFrame *frame;
    QHBoxLayout *horizontalLayout;
    QLabel *armedStatus;
    QLabel *label_8;
    QLabel *modeValue;
    QSpacerItem *horizontalSpacer;
    QListWidget *gcsLog;
    QFrame *frame_2;
    QHBoxLayout *horizontalLayout_3;
    QLabel *attitude;
    QLabel *speeds;
    QSpacerItem *horizontalSpacer_2;
    QCheckBox *logAutoScroll;
    QMenuBar *menubar;
    QMenu *menuParameters_set;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 669);
        actionParameters_set = new QAction(MainWindow);
        actionParameters_set->setObjectName("actionParameters_set");
        actionRefresh_configs = new QAction(MainWindow);
        actionRefresh_configs->setObjectName("actionRefresh_configs");
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName("gridLayout");
        label_7 = new QLabel(centralwidget);
        label_7->setObjectName("label_7");

        gridLayout->addWidget(label_7, 1, 0, 1, 1);

        frame = new QFrame(centralwidget);
        frame->setObjectName("frame");
        frame->setFrameShape(QFrame::Shape::StyledPanel);
        frame->setFrameShadow(QFrame::Shadow::Raised);
        horizontalLayout = new QHBoxLayout(frame);
        horizontalLayout->setObjectName("horizontalLayout");
        armedStatus = new QLabel(frame);
        armedStatus->setObjectName("armedStatus");

        horizontalLayout->addWidget(armedStatus);

        label_8 = new QLabel(frame);
        label_8->setObjectName("label_8");

        horizontalLayout->addWidget(label_8);

        modeValue = new QLabel(frame);
        modeValue->setObjectName("modeValue");

        horizontalLayout->addWidget(modeValue);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        gridLayout->addWidget(frame, 0, 0, 1, 1);

        gcsLog = new QListWidget(centralwidget);
        gcsLog->setObjectName("gcsLog");
        gcsLog->setAutoScroll(false);

        gridLayout->addWidget(gcsLog, 2, 0, 1, 1);

        frame_2 = new QFrame(centralwidget);
        frame_2->setObjectName("frame_2");
        frame_2->setFrameShape(QFrame::Shape::StyledPanel);
        frame_2->setFrameShadow(QFrame::Shadow::Raised);
        horizontalLayout_3 = new QHBoxLayout(frame_2);
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        attitude = new QLabel(frame_2);
        attitude->setObjectName("attitude");

        horizontalLayout_3->addWidget(attitude);

        speeds = new QLabel(frame_2);
        speeds->setObjectName("speeds");

        horizontalLayout_3->addWidget(speeds);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_2);

        logAutoScroll = new QCheckBox(frame_2);
        logAutoScroll->setObjectName("logAutoScroll");
        logAutoScroll->setChecked(false);

        horizontalLayout_3->addWidget(logAutoScroll);


        gridLayout->addWidget(frame_2, 3, 0, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 19));
        menuParameters_set = new QMenu(menubar);
        menuParameters_set->setObjectName("menuParameters_set");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuParameters_set->menuAction());
        menuParameters_set->addAction(actionParameters_set);
        menuParameters_set->addAction(actionRefresh_configs);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        actionParameters_set->setText(QCoreApplication::translate("MainWindow", "Parameters set", nullptr));
        actionRefresh_configs->setText(QCoreApplication::translate("MainWindow", "Refresh configs", nullptr));
        label_7->setText(QCoreApplication::translate("MainWindow", "Log", nullptr));
        armedStatus->setText(QCoreApplication::translate("MainWindow", "DISARMED", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "Mode:", nullptr));
        modeValue->setText(QCoreApplication::translate("MainWindow", "None", nullptr));
        attitude->setText(QCoreApplication::translate("MainWindow", "R: 0.0\302\260, P: 0.0\302\260, Y: 0.0\302\260", nullptr));
        speeds->setText(QCoreApplication::translate("MainWindow", "FS: 0.0 m/s, LS: 0.0 m/s, VS: 0.0 m/s", nullptr));
        logAutoScroll->setText(QCoreApplication::translate("MainWindow", "Log auto scroll", nullptr));
        menuParameters_set->setTitle(QCoreApplication::translate("MainWindow", "Settings", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
