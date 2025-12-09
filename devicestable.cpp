#include "devicestable.h"
#include "ui_devicestable.h"

#include <QTableWidget>

#include "mavlinkdevice.h"

DevicesTable::DevicesTable(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DevicesTable)
{
    ui->setupUi(this);
}

DevicesTable::~DevicesTable() {
    delete ui;
}

void DevicesTable::onDeviceConnected(MavlinkDevice* device) {
    QList<QTableWidgetItem*> items = ui->tableWidget->findItems(device->getName(), Qt::MatchExactly);
    if (items.size() > 0) {
        qDebug() << "Device is already added";
        return;
    }
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 0, new QTableWidgetItem(device->getName()));
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 1, new QTableWidgetItem(device->getType()));
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 2, new QTableWidgetItem("Conecting..."));
}

void DevicesTable::onDeviceDisconnected(MavlinkDevice* device) {

}

void DevicesTable::onDeviceStateChanged(MavlinkDevice* device, PortState state) {
    QList<QTableWidgetItem*> items = ui->tableWidget->findItems(device->getName(), Qt::MatchExactly);
    if (items.size() == 0) {
        qDebug() << "Device soesn't exist";
        return;
    }
    int32_t row = items[0]->row();
    switch (state) {
    case PortState::Uninitialized:
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem("Uninitialized"));
        break;
    case PortState::Initialized:
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem("Initialized"));
        break;
    case PortState::Opened:
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem("Opened"));
        break;
    default:
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem("Error"));
    }
}
