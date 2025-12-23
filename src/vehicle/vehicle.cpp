#include "vehicle.h"

#include "protocols/basepacketizer.h"
#include "deviceManagement/basedevice.h"
#include "./logs/vehiclelogs.h"
#include "./telemetry/vehicletelemetry.h"
#include "./statuslog/vehiclestatuslog.h"
#include "./parameters/Parameters.h"

Vehicle::Vehicle(BasePacketizer* packetizer, QObject* parent) 
    : QObject(parent), _packetizer((packetizer)) {}

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
    if (_parameters) {
        _parameters->deleteLater();
    }
    if(_active) {
        _active->quit();
        _active->wait();
    }
    for(auto device : _devices) {
        device->quit();
        device->wait();
    }
    _devices.clear();
}

BasePacketizer* Vehicle::getPacketizer() {
    return _packetizer;
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

void Vehicle::setActiveDevice(BaseDevice* device) {
    if(_active) {
        disconnect(_packetizer, &BasePacketizer::messageTransmitRequest, _active, &BaseDevice::onSendRawCommand);
        disconnect(_active, &BaseDevice::portOpened, _parameters, &Parameters::onConnect);
        disconnect(_active, &BaseDevice::portClosed, _parameters, &Parameters::onDisconnect);
    }
    
    _active = device;
    _active->setParent(this);
    _packetizer->moveToThread(_active);
    connect(_packetizer, &BasePacketizer::messageTransmitRequest, _active, &BaseDevice::onSendRawCommand);
    connect(_active, &BaseDevice::portOpened, _parameters, &Parameters::onConnect);
    connect(_active, &BaseDevice::portClosed, _parameters, &Parameters::onDisconnect);
}
