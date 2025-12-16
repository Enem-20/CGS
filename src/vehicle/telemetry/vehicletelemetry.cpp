#include "vehicletelemetry.h"

#include <QDebug>

#include <cmath>
#include <common/mavlink.h>

VehicleTelemetry::VehicleTelemetry(QObject *parent)
    : QObject{parent}
{}

