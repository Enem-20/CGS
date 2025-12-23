#include "devicestable.h"
#include "ui_devicestable.h"

#include <QTableWidget>

DevicesTable::DevicesTable(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DevicesTable)
{
    ui->setupUi(this);
}

DevicesTable::~DevicesTable() {
    delete ui;
}

void DevicesTable::onDeviceConnected(QStringView name, QStringView type) {
    QList<QTableWidgetItem*> items = ui->table->findItems(name.toString(), Qt::MatchExactly);
    if (items.size() > 0) {
        qDebug() << "Device is already added";
        return;
    }
    ui->table->insertRow(ui->table->rowCount());
    ui->table->setItem(ui->table->rowCount()-1, 0, new QTableWidgetItem(name.toString()));
    ui->table->setItem(ui->table->rowCount()-1, 1, new QTableWidgetItem(type.toString()));
    ui->table->setItem(ui->table->rowCount()-1, 2, new QTableWidgetItem("Conecting..."));
}

void DevicesTable::onDeviceDisconnected(QStringView name) {
    QList<QTableWidgetItem*> items = ui->table->findItems(name.toString(), Qt::MatchExactly);
    if (items.size() > 0) {
        qDebug() << "Device doesn't exist";
        return;
    }
    ui->table->removeRow(items[0]->row());
}

void DevicesTable::onDeviceStateChanged(QStringView name) {
    QList<QTableWidgetItem*> items = ui->table->findItems(name.toString(), Qt::MatchExactly);
    if (items.size() == 0) {
        qDebug() << "Device doesn't exist";
        return;
    }
    int32_t row = items[0]->row();
}

void DevicesTable::on_table_itemDoubleClicked(QTableWidgetItem *item) {
    emit makeDeviceActive(ui->table->item(item->row(), 0)->text());
}

