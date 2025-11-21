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
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QListWidget *listWidget;
    QFrame *frame;
    QHBoxLayout *horizontalLayout;
    QLabel *label_8;
    QLabel *label_9;
    QSpacerItem *horizontalSpacer;
    QLabel *label;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_7;
    QMenuBar *menubar;
    QMenu *menuParameters_set;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        actionParameters_set = new QAction(MainWindow);
        actionParameters_set->setObjectName("actionParameters_set");
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName("gridLayout");
        listWidget = new QListWidget(centralwidget);
        listWidget->setObjectName("listWidget");

        gridLayout->addWidget(listWidget, 2, 0, 1, 1);

        frame = new QFrame(centralwidget);
        frame->setObjectName("frame");
        frame->setFrameShape(QFrame::Shape::StyledPanel);
        frame->setFrameShadow(QFrame::Shadow::Raised);
        horizontalLayout = new QHBoxLayout(frame);
        horizontalLayout->setObjectName("horizontalLayout");
        label_8 = new QLabel(frame);
        label_8->setObjectName("label_8");

        horizontalLayout->addWidget(label_8);

        label_9 = new QLabel(frame);
        label_9->setObjectName("label_9");

        horizontalLayout->addWidget(label_9);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        label = new QLabel(frame);
        label->setObjectName("label");

        horizontalLayout->addWidget(label);

        label_4 = new QLabel(frame);
        label_4->setObjectName("label_4");

        horizontalLayout->addWidget(label_4);

        label_5 = new QLabel(frame);
        label_5->setObjectName("label_5");

        horizontalLayout->addWidget(label_5);

        label_6 = new QLabel(frame);
        label_6->setObjectName("label_6");

        horizontalLayout->addWidget(label_6);

        label_2 = new QLabel(frame);
        label_2->setObjectName("label_2");

        horizontalLayout->addWidget(label_2);

        label_3 = new QLabel(frame);
        label_3->setObjectName("label_3");

        horizontalLayout->addWidget(label_3);


        gridLayout->addWidget(frame, 0, 0, 1, 1);

        label_7 = new QLabel(centralwidget);
        label_7->setObjectName("label_7");

        gridLayout->addWidget(label_7, 1, 0, 1, 1);

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

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        actionParameters_set->setText(QCoreApplication::translate("MainWindow", "Parameters set", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "Mode:", nullptr));
        label_9->setText(QCoreApplication::translate("MainWindow", "None", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "R:", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "0.0", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "P:", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "0.0", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Y:", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "0.0", nullptr));
        label_7->setText(QCoreApplication::translate("MainWindow", "Log", nullptr));
        menuParameters_set->setTitle(QCoreApplication::translate("MainWindow", "Settings", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
