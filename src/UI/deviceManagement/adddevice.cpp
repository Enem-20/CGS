#include "adddevice.h"
#include "ui_adddevice.h"

AddDevice::AddDevice(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AddDevice)
{
    ui->setupUi(this);
}

AddDevice::~AddDevice()
{
    delete ui;
}

void AddDevice::on_cancel_clicked()
{
    emit transitToDevicesTable();
}


void AddDevice::on_ok_clicked()
{
    emit transitToDevicesTable();
}

