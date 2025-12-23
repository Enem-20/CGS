#include "VehicleManager.h"

#include <QDebug>

#include "vehicle/PhysicalVehicle.h"

VehicleManager* VehicleManager::_instance;

VehicleManager::VehicleManager() {

}

VehicleManager* VehicleManager::getInstance() {
    if(_instance) [[likely]]
        return _instance;
    _instance = new VehicleManager();
    _instance->start(QThread::LowPriority);
    return _instance;
}

void VehicleManager::onVehicleDetected(VehicleId id) {
    if (contains(id)) {
        return;
    }
    
    PhysicalVehicle* vehicle = new PhysicalVehicle();
    vehicle->registerId(id);

    _vehicles.emplace(id.uniqueVehicleId, vehicle);
}

void VehicleManager::onVehicleLost(VehicleId id) {
    auto it = _vehicles.find(id.uniqueVehicleId);
    if (it == _vehicles.end()) {
        return;
    }

    PhysicalVehicle* vehicle = it.value();
    vehicle->deactivateId(id);
}

void VehicleManager::onVehicleConnect(VehicleId id) {
    auto it = _vehicles.find(id.uniqueVehicleId);
    if (it == _vehicles.end()) {
        return;
    }
}

void VehicleManager::onVehicleDisconnect(VehicleId id) {
    auto it = _vehicles.find(id.uniqueVehicleId);
    if (it == _vehicles.end()) {
        return;
    }
    
}

bool VehicleManager::contains(VehicleId id) const {
    return _vehicles.contains(id.uniqueVehicleId) && _vehicles[id.uniqueVehicleId]->is(id);
}

void VehicleManager::create(VehicleId id) {
    // PhysicalVehicle* vehicle = nullptr;
    // switch (id.protocolId) {
    // case 0:
    //     vehicle = new PhysicalVehicle();
    //     _vehicles.emplace(id.uniqueVehicleId, vehicle);
    // break;
    // default:
    //     qWarning() << "Failed to create vehicle: Unknown protocol id";
    // break;
    // }
}
