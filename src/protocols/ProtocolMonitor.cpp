#include "ProtocolMonitor.h"

#include <common/mavlink.h>

#include "mavlink/mavlinkpacketizer.h"
#include "vehicleManagement/VehicleManager.h"
#include "deviceManagement/basedevice.h"

ProtocolMonitor::ProtocolMonitor(BaseDevice* device) 
    : _device(device)
{
    _mavlinkPacketizer = new MavlinkPacketizer(this);
    VehicleManager* manager = VehicleManager::getInstance();
    connect(_device, &BaseDevice::byteReceived, _mavlinkPacketizer, &MavlinkPacketizer::onPushByte);
    connect(_mavlinkPacketizer, &MavlinkPacketizer::heartbeatReceived, this, &ProtocolMonitor::onHeartbeatMessage);
    // connect(this, &ProtocolMonitor::vehicleDetected, manager, &VehicleManager::create);
    // connect(this, &ProtocolMonitor::checkContainsBlockable, manager, &VehicleManager::contains, Qt::BlockingQueuedConnection);
    connect(this, &ProtocolMonitor::vehicleLost, this, &ProtocolMonitor::onVehicleLost);
    connect(this, &ProtocolMonitor::vehicleLost, manager, &VehicleManager::onVehicleLost);
    connect(this, &ProtocolMonitor::vehicleDetected, manager, &VehicleManager::onVehicleDetected);
}

void ProtocolMonitor::onHeartbeatMessage(Message msg) {
    mavlink_heartbeat_t heartbeat{};
    mavlink_message_t* converted = msg.read<mavlink_message_t>();
    mavlink_msg_heartbeat_decode(converted, &heartbeat);

    switch (heartbeat.autopilot) {
    case MAV_AUTOPILOT_INVALID:
        return;
    break;
    }

    VehicleManager* manager = VehicleManager::getInstance();

    VehicleId id;
    id.protocolId = 0;
    id.uniqueVehicleId = converted->sysid;
    id.deviceType = _device->getTypeHash();
    id.deviceId = reinterpret_cast<uint64_t>(_device);

    auto vehicleTimer = _detectedVehiclesTimers.find(id);
    if (vehicleTimer == _detectedVehiclesTimers.end()) {
        QTimer* vehicleConnectionTimeout = new QTimer();
        vehicleConnectionTimeout->start(5000);
        connect(vehicleConnectionTimeout, &QTimer::timeout, [this, id](){
            emit vehicleLost(id);
        });
        _detectedVehiclesTimers.insert(id, vehicleConnectionTimeout);
        emit vehicleDetected(id);
    }
    else {
        vehicleTimer.value()->start();
    }

    // emit packetizerUpdate(new MavlinkPacketizer());

    // bool result = emit checkContainsBlockable(id);
    // if(!result) {
    //     emit createVehicle(id);
    // }
}

void ProtocolMonitor::onVehicleLost(VehicleId id) {
    auto _vehicleIt = _detectedVehiclesTimers.find(id);
    if(_vehicleIt != _detectedVehiclesTimers.end()) {
        delete _vehicleIt.value();
        _detectedVehiclesTimers.remove(id);
    }
}
