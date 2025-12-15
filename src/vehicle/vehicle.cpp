#include "vehicle.h"

#include "./logs/vehiclelogs.h"

Vehicle::Vehicle(QObject *parent)
    : QObject{parent}
{

}

Vehicle::~Vehicle() {
    if (_logs) {
        delete _logs;
    }
}

void Vehicle::setDevice(BaseDevice* device) {
    _device = device;
}

VehicleLogs* Vehicle::getLogs() {
    return _logs;
}
