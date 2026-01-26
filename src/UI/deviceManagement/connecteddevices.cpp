#include "connecteddevices.h"
#include "ui_connecteddevices.h"

ConnectedDevices::ConnectedDevices(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ConnectedDevices)
{
    ui->setupUi(this);
}

ConnectedDevices::~ConnectedDevices()
{
    delete ui;
}
