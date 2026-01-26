#include "mavlinkpacketizer.h"

#include <common/mavlink.h>

#include "../message.h"
#include "tools/hashes.h"

void MavlinkPacketizer::sendEmits(uint32_t msgId, Message msg) {
    switch(msgId) {
        case MAVLINK_MSG_ID_HEARTBEAT:
            emit heartbeatReceived(msg);
        break; 
        case MAVLINK_MSG_ID_PARAM_VALUE:
            emit paramValueReceived(msg);
        break;
        case MAVLINK_MSG_ID_LOG_ENTRY:
            emit logEntryReceived(msg);
        break;  
        case MAVLINK_MSG_ID_LOG_DATA:
            emit logDataReceived(msg);
        break;  
        case MAVLINK_MSG_ID_STATUSTEXT:
            emit statusTextReceived(msg);
        break;  
        case MAVLINK_MSG_ID_ATTITUDE:
            emit attitudeReceived(msg);
        break; 
        case MAVLINK_MSG_ID_LOCAL_POSITION_NED:
            emit localPositionNEDReceived(msg);
        break; 
        case MAVLINK_MSG_ID_GLOBAL_POSITION_INT:
            emit globalPositionINTReceived(msg);
        break; 
    }
}

MavlinkPacketizer::MavlinkPacketizer(QObject *parent)
    : BasePacketizer(GENERATE_HASH(MavlinkPacketizer), parent)
    , _msg(new mavlink_message_t{})
    , _status(new mavlink_status_t{})
{
    connect(this, &MavlinkPacketizer::heartbeatReceived, this, &MavlinkPacketizer::onHeartbeat);
}

MavlinkPacketizer::~MavlinkPacketizer() {
    if(_msg)
        delete _msg;
    if(_status)
        delete _status;
}

void MavlinkPacketizer::onHeartbeat(Message msg) {
    
}

void MavlinkPacketizer::onSendMessageRequest(Message msg) {
    emit messageTransmitRequest(packagePrepare(msg));
}

void MavlinkPacketizer::onPushByte(uint8_t byte) {
    bool isSuccessfulyParsed = mavlink_parse_char(MAVLINK_COMM_0, byte, _msg, _status);
    if (isSuccessfulyParsed) {
        Message msg;
        msg.write(_msg);
        sendEmits(_msg->msgid, msg);

        emit messageReceived(msg);
        delete _msg;
        delete _status;
        _msg = new mavlink_message_t{};
        _status = new mavlink_status_t{};
        emit successfullyParsed();
    }   else emit notSuccessfullyParsed();
}

QByteArray MavlinkPacketizer::packagePrepare(Message msg) {
    QByteArray result;
    result.resize(sizeof(mavlink_message_t));
    mavlink_message_t* mavMsg = msg.read<mavlink_message_t>();
    //avMsg->sysid = _
    std::memcpy(result.data(), mavMsg, sizeof(mavlink_message_t));
    return result;
}
