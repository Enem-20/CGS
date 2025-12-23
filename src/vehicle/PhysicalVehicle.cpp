#include "PhysicalVehicle.h"

#include <QDebug>

#include "vehicle.h"
#include "deviceManagement/udp/udpdevice.h"
#include "deviceManagement/serial/serialdevice.h"
#include "vehicle/mavlink/mavlinkvehicle.h"
#include "protocols/mavlink/mavlinkpacketizer.h"

PhysicalVehicle::PhysicalVehicle() {

}

uint64_t deternineDeviceType(BaseDevice* device) {
    UDPDevice* udpDevice = qobject_cast<UDPDevice*>(device);
    SerialDevice* serialDevice = qobject_cast<SerialDevice*>(device);
    if(udpDevice)
        return 0;
    else if (serialDevice)
        return 1;
    return -1;
}

// void PhysicalVehicle::onVehicleId(VehicleId id, BaseDevice* device) {
//     auto virtualVehicleIt = _virtualVehicles.find(id);
//     if (virtualVehicleIt != _virtualVehicles.end()) {
//         Vehicle* vehicle = nullptr;
//         switch (id.protocolId) {
//         case 0:
//             vehicle = new MavlinkVehicle();
//         default:
//             qWarning() << "Trying to add unsupported device protocol";
//         }
//         if (vehicle != nullptr)
//             _virtualVehicles.emplace(id, vehicle);
//     }
// }

void PhysicalVehicle::registerId(VehicleId id) {
    auto it = _registeredIds.find(id);
    if (!_registeredIds.contains(id)) {
        createVehicle(id);
        _registeredIds.insert(id, true);
        return;
    }
    it.value() = true;
}

void PhysicalVehicle::deactivateId(VehicleId id) {
    auto it = _registeredIds.find(id);
    if (!_registeredIds.contains(id)) {
        return;
    }
    it.value() = false;
}

void PhysicalVehicle::unregisterId(VehicleId id) {
    if (_registeredIds.contains(id)) {
        _registeredIds.remove(id);
        destroyVehicle(id);
    }
}

bool PhysicalVehicle::is(VehicleId id) {
    return _registeredIds.contains(id);
}

bool PhysicalVehicle::hasRegisteredIds() {
    return _registeredIds.size() > 0;
}

void PhysicalVehicle::onEnable() {
    if (!_enabled) {
        _enabled = true;
        emit enabled(this);
    }
}

void PhysicalVehicle::onDisable() {
    if (_enabled) {
        _enabled = false;
        emit disabled(this);
    }
}

void PhysicalVehicle::onVehicleId(VehicleId id, BaseDevice* device) {

}

void PhysicalVehicle::createVehicle(VehicleId id) {
    if (!_virtualVehicles.contains(id.protocolId)) [[unlikely]] {
        _virtualVehicles.insert(id.protocolId, {});
    }
    DeviceTypeMap& selectedProtocol = _virtualVehicles[id.protocolId];
    if (!selectedProtocol.contains(id.deviceType)) [[unlikely]] {
        selectedProtocol.insert(id.deviceType, {});
    }
    DeviceMap& selectedDeviceType = selectedProtocol[id.deviceType];
    if (!selectedDeviceType.contains(id.deviceId)) [[unlikely]] {
        BasePacketizer* packetizer = nullptr;
        if (id.protocolId == 0) {
            packetizer = new MavlinkPacketizer();
        }
        else {
            qWarning() << "Failed to create packetizer for unrecognized protocol id: " << id.protocolId;
        }
        Vehicle* vehicle = new Vehicle(packetizer);

        selectedDeviceType.insert(id.deviceId, vehicle);
    }
    else {
        qWarning() << "Creating vehicle that already exists: " 
                   << id.protocolId << " " << id.deviceType << " " << id.deviceId << " " << id.uniqueVehicleId;
    }
}

void PhysicalVehicle::destroyVehicle(VehicleId id) {
    if (!_virtualVehicles.contains(id.protocolId)) [[unlikely]] {
        qWarning() << "Destroying vehicle that doesn't exists: " 
                   << id.protocolId << " " << id.deviceType << " " << id.deviceId << " " << id.uniqueVehicleId;
        return;
    }
    DeviceTypeMap& selectedProtocol = _virtualVehicles[id.protocolId];
    if (!selectedProtocol.contains(id.deviceType)) [[unlikely]] {
        qWarning() << "Destroying vehicle that doesn't exists: " 
                   << id.protocolId << " " << id.deviceType << " " << id.deviceId << " " << id.uniqueVehicleId;
        return;
    }
    DeviceMap& selectedDeviceType = selectedProtocol[id.deviceType];
    if (!selectedDeviceType.contains(id.deviceId)) [[unlikely]] {
        qWarning() << "Destroying vehicle that doesn't exists: " 
                   << id.protocolId << " " << id.deviceType << " " << id.deviceId << " " << id.uniqueVehicleId;
        return;
    }
    else {
        selectedDeviceType[id.deviceId]->deleteLater();
        selectedDeviceType.remove(id.deviceId);
    }
}
