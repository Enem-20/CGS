#include "ProtocolMonitor.h"

#include <common/mavlink.h>

#include "channelManagement/ChannelManager.h"
#include "channelManagement/RemoteChannelID.h"
#include "mavlink/mavlinkpacketizer.h"
#include "deviceManagement/basedevice.h"
#include "protocols/mavlink/MavlinkProtocol.h"

ProtocolMonitor::ProtocolMonitor(QObject* parent) 
    : QObject(parent)
    , _device(qobject_cast<BaseDevice*>(parent))
{
    _mavlinkPacketizer = new MavlinkPacketizer(this);

    
    // connect(this, &ProtocolMonitor::vehicleDetected, manager, &VehicleManager::create);
    // connect(this, &ProtocolMonitor::checkContainsBlockable, manager, &VehicleManager::contains, Qt::BlockingQueuedConnection);
    //connect(this, &ProtocolMonitor::channelLost, this, &ProtocolMonitor::onChannelLost);
}

ProtocolMonitor::~ProtocolMonitor() {
    if (_mavlinkPacketizer) {
        _mavlinkPacketizer->deleteLater();
    }
}

void ProtocolMonitor::init() {
    ChannelManager* manager = ChannelManager::getInstance();
    connect(this, &ProtocolMonitor::channelDetected, manager, &ChannelManager::onChannelDetected);
    connect(this, &ProtocolMonitor::channelLost, manager, &ChannelManager::onChannelLost);
    connect(_device, &BaseDevice::byteReceived, _mavlinkPacketizer, &MavlinkPacketizer::onPushByte);
    connect(_mavlinkPacketizer, &MavlinkPacketizer::heartbeatReceived, this, &ProtocolMonitor::onHeartbeatMessage);

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

    RemoteChannelID id;
    id._protocolID = MavlinkProtocol::getInstance<MavlinkProtocol>()->getTypeHash();
    id._sourceID = converted->sysid;
    id._portType = _device->getTypeHash();
    id._portID = reinterpret_cast<uint64_t>(_device);

    auto channelTimer = _detectedChannelsTimers.find(id);
    if (channelTimer == _detectedChannelsTimers.end()) {
        QTimer* channelConnectionTimeout = new QTimer();
        channelConnectionTimeout->start(5000);
        connect(channelConnectionTimeout, &QTimer::timeout, [this, id](){
            emit channelLost(id);
        });
        _detectedChannelsTimers.insert(id, channelConnectionTimeout);
        emit channelDetected(id);
    }
    else {
        channelTimer.value()->start();
    }
}

void ProtocolMonitor::onChannelLost(RemoteChannelID id) {
    auto channelIDIt = _detectedChannelsTimers.find(id);
    if(channelIDIt != _detectedChannelsTimers.end()) {
        delete channelIDIt.value();
        _detectedChannelsTimers.remove(id);
    }
}
