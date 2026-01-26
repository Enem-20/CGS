#include "vehicletelemetry.h"

#include <QDebug>

#include <cmath>
#include <common/mavlink.h>

VehicleTelemetry::VehicleTelemetry(QObject *parent)
    : ProtocolSubscriber{parent}
{}

void VehicleTelemetry::onStartTelemetry(){
    requestTelemetry();
}

void VehicleTelemetry::onStopTelemetry() {
    requestStopTelemetry();
}
