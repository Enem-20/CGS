#include "vehicle.h"

Vehicle::Vehicle(QObject *parent)
    : QObject{parent}
{

}

void Vehicle::setDevice(BaseDevice* device) {
    _device = device;
}
