#include "mavlinkvehicle.h"

#include "common/mavlink.h"
#include "protocols/message.h"

MavlinkVehicle::MavlinkVehicle(QObject *parent)
    : Vehicle{parent}
{

}

void MavlinkVehicle::getLog(size_t index) {
    mavlink_message_t* command = new mavlink_message_t{};
    mavlink_msg_log_request_data_pack(
        255,
        MAV_COMP_ID_MISSIONPLANNER,
        command,
        _sysId,
        _compId,
        static_cast<uint16_t>(index),
        0,
        entry->size
        );
    Message msg([](void* msg){
        return nullptr;
    },[](void* msg){});
    msg.write<mavlink_message_t>(command);
}

void MavlinkVehicle::getLogs() {

}
