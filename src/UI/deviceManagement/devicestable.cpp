#include "devicestable.h"
#include "ui_devicestable.h"

#include <QLineEdit>
#include <QTableWidget>
#include <QPushButton>

DevicesTable::DevicesTable(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DevicesTable)
{
    ui->setupUi(this);
    connect(ui->filter, &QLineEdit::textEdited, this, [this](const QString& text){
        if (text == "") { showAll(); return; }
        hideAll();

        QList<QTableWidgetItem*> matches = ui->table->findItems(text, Qt::MatchRegularExpression);
        for (size_t i = 0; i < matches.size(); ++i) {
            ui->table->showRow(matches[i]->row());
        }
    });

    connect(ui->add, &QPushButton::clicked, this, [this](){
        emit transitToAddDevice();
    });
}

DevicesTable::~DevicesTable() {
    delete ui;
}

void DevicesTable::showAll() {
    for(size_t i = 0; i < ui->table->rowCount(); ++i) {
        ui->table->showRow(i);
    }
}

void DevicesTable::hideAll() {
        for(size_t i = 0; i < ui->table->rowCount(); ++i) {
        ui->table->hideRow(i);
    }
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
    ui->table->setItem(ui->table->rowCount()-1, 2, new QTableWidgetItem("Connecting..."));
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


void DevicesTable::on_add_clicked()
{

}

