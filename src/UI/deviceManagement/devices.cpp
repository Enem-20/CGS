#include "devices.h"
#include "ui_devices.h"

#include "devicestable.h"
#include "adddevice.h"

Devices::Devices(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Devices)
{
    ui->setupUi(this);
    ui->replacable = _devicesTable = new DevicesTable(this);
    _addDevice = new AddDevice(this);
    _addDevice->hide();
    connect(qobject_cast<DevicesTable*>(_devicesTable), &DevicesTable::transitToAddDevice, this, &Devices::onTransitToAddDevice);
    connect(qobject_cast<AddDevice*>(_addDevice), &AddDevice::transitToDevicesTable, this, &Devices::onTransitToDevicesTable);
}

Devices::~Devices()
{
    delete ui;
}

void Devices::transitToWidget(QWidget* widget) {
    ui->replacable->hide();
    ui->replacable = widget;
    ui->replacable->show();
}

void Devices::onTransitToAddDevice() {
    transitToWidget(_addDevice);
}

void Devices::onTransitToDevicesTable() {
    transitToWidget(_devicesTable);
}
