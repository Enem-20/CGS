#include "defaultdevicewidget.h"
#include "ui_defaultdevicewidget.h"

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
    emit makeDefaultDeviceActive(QString("defaultDevice"));
}

void DefaultDeviceWidget::onActiveDeviceChanged(QStringView deviceName) {
    if (deviceName != "defaultDevice") {
        ui->buttonConnect->setEnabled(true);
    } else {
        ui->buttonConnect->setEnabled(false);
    }
}
