#include "defaultdevicewidget.h"
#include "ui_defaultdevicewidget.h"

#include "mavlink/mavlinkdevice.h"

DefaultDeviceWidget::DefaultDeviceWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DefaultDeviceWidget)
{
    ui->setupUi(this);
}

DefaultDeviceWidget::~DefaultDeviceWidget()
{
    delete ui;
}

void DefaultDeviceWidget::on_buttonConnect_clicked() {
    emit makeDefaultDeviceActive(DEFAULT_DEVICE_NAME);
}

void DefaultDeviceWidget::onActiveDeviceChanged(QStringView deviceName) {
    if (deviceName != DEFAULT_DEVICE_NAME) {
        ui->buttonConnect->setEnabled(true);
    } else {
        ui->buttonConnect->setEnabled(false);
    }
}
