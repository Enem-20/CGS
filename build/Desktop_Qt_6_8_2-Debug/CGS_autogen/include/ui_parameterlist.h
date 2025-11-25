/********************************************************************************
** Form generated from reading UI file 'parameterlist.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PARAMETERLIST_H
#define UI_PARAMETERLIST_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ParameterList
{
public:
    QGridLayout *gridLayout;
    QTableWidget *tableWidget;
    QFrame *frame;
    QVBoxLayout *verticalLayout;
    QPushButton *syncWithVehicle;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *ParameterList)
    {
        if (ParameterList->objectName().isEmpty())
            ParameterList->setObjectName("ParameterList");
        ParameterList->resize(827, 534);
        gridLayout = new QGridLayout(ParameterList);
        gridLayout->setObjectName("gridLayout");
        tableWidget = new QTableWidget(ParameterList);
        if (tableWidget->columnCount() < 2)
            tableWidget->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        tableWidget->setObjectName("tableWidget");
        tableWidget->setSortingEnabled(true);

        gridLayout->addWidget(tableWidget, 0, 0, 1, 1);

        frame = new QFrame(ParameterList);
        frame->setObjectName("frame");
        frame->setFrameShape(QFrame::Shape::StyledPanel);
        frame->setFrameShadow(QFrame::Shadow::Raised);
        verticalLayout = new QVBoxLayout(frame);
        verticalLayout->setObjectName("verticalLayout");
        syncWithVehicle = new QPushButton(frame);
        syncWithVehicle->setObjectName("syncWithVehicle");

        verticalLayout->addWidget(syncWithVehicle);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        gridLayout->addWidget(frame, 0, 1, 1, 1);


        retranslateUi(ParameterList);

        QMetaObject::connectSlotsByName(ParameterList);
    } // setupUi

    void retranslateUi(QWidget *ParameterList)
    {
        ParameterList->setWindowTitle(QCoreApplication::translate("ParameterList", "Form", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("ParameterList", "Parameter", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("ParameterList", "Value", nullptr));
        syncWithVehicle->setText(QCoreApplication::translate("ParameterList", "Sync with vehicle", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ParameterList: public Ui_ParameterList {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PARAMETERLIST_H
