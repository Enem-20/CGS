#include "mavlinkpacketizer.h"

#include <common/mavlink.h>

#include "../message.h"

MavlinkPacketizer::MavlinkPacketizer(QObject *parent)
    : BasePacketizer(parent)
    , _msg(new mavlink_message_t{})
    , _status(new mavlink_status_t{})
{

}

MavlinkPacketizer::~MavlinkPacketizer() {
    if(_msg)
        delete _msg;
    if(_status)
        delete _status;
}

void MavlinkPacketizer::onSendMessageRequest(Message msg) {

}

bool MavlinkPacketizer::onPushByte(uint8_t byte) {
    bool isSuccessfulyParsed = mavlink_parse_char(MAVLINK_COMM_0, byte, _msg, _status);
    if (isSuccessfulyParsed) {
        Message msg(
                    [](void* msg){
                        mavlink_message_t* msgTyped = reinterpret_cast<mavlink_message_t*>(msg);
                        mavlink_message_t* result = nullptr;
                        std::memcpy(result, msgTyped, sizeof(mavlink_message_t));
                        return reinterpret_cast<void*>(result);
                    },
                    [](void* msg){delete reinterpret_cast<mavlink_message_t*>(msg);},
                    reinterpret_cast<void*>(_msg)
        );
        emit messageReceived(msg);
        _msg = new mavlink_message_t{};
        delete _status;
        _status = new mavlink_status_t{};
    }
    return isSuccessfulyParsed;
}

QByteArray MavlinkPacketizer::packagePrepare(Message msg) {
    QByteArray result;
    result.resize(sizeof(mavlink_message_t));
    mavlink_message_t* mavMsg = msg.read<mavlink_message_t>();
    std::memcpy(result.data(), mavMsg, sizeof(mavlink_message_t));
    return result;
}
