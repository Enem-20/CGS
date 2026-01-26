#include "MavlinkProtocolFabric.h"

#include "vehicle/mavlink/mavlinkvehicle.h"
#include "channelManagement/ChannelManager.h"

MavlinkProtocolFabric::MavlinkProtocolFabric(QObject* parent) 
    : ProtocolFabric(parent)
{}

Vehicle* MavlinkProtocolFabric::createVehicle(RemoteChannelID id, QList<uint64_t> layers) {
    return nullptr;
    //return new MavlinkVehicle(ChannelManager::getInstance());
}