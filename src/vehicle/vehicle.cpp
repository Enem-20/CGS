#include "vehicle.h"

#include "protocols/basepacketizer.h"

#include "./logs/vehiclelogs.h"
#include "./telemetry/vehicletelemetry.h"
#include "./statuslog/vehiclestatuslog.h"
#include "./parameters/Parameters.h"

Vehicle::Vehicle(QObject* parent) 
    : QObject(parent) {}

Vehicle::~Vehicle() {
    if (_logs) {
        _logs->deleteLater();
    }
    if (_telemetry) {
        _telemetry->deleteLater();
    }
    if (_statusLog) {
        _statusLog->deleteLater();
    }
    if(_parameters) {
        _parameters->deleteLater();
    }
}

VehicleLogs* Vehicle::getLogs() {
    return _logs;
}

VehicleTelemetry* Vehicle::getTelemetry() {
    return _telemetry;
}

VehicleStatusLog* Vehicle::getStatusLog() {
    return _statusLog;
}

Parameters* Vehicle::getParameters() {
    return _parameters;
}

void Vehicle::setDevice(BaseDevice* device) {
    _device = device;
}

void Vehicle::setPacketizer(BasePacketizer* packetizer) {
    _packetizer = packetizer;
}
