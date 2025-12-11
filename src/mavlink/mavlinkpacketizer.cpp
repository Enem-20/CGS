#include "mavlinkpacketizer.h"

#include <common/mavlink.h>

#include <QDebug>

MavlinkPacketizer::MavlinkPacketizer(QObject *parent)
    : QObject{parent}
{

}

void MavlinkPacketizer::mavlinkMsgReceived(const mavlink_message_t& msg) {
    emit mavlinkMessageUpdated(msg);
    switch(msg.msgid) {
    case MAVLINK_MSG_ID_HEARTBEAT:
        mavlink_heartbeat_t heartbeat;
        mavlink_msg_heartbeat_decode(&msg, &heartbeat);
        emit heartbeatUpdated(heartbeat);
    break;
    case MAVLINK_MSG_ID_ATTITUDE:
        mavlink_attitude_t attitude;
        mavlink_msg_attitude_decode(&msg, &attitude);
        emit attitudeUpdated(attitude);
    break;
    case MAVLINK_MSG_ID_LOCAL_POSITION_NED:
        mavlink_local_position_ned_t localPositionNED;
        mavlink_msg_local_position_ned_decode(&msg, &localPositionNED);
        emit localPositionNEDUpdated(localPositionNED);
    break;
    case MAVLINK_MSG_ID_GLOBAL_POSITION_INT:
        mavlink_global_position_int_t globalPositionInt;
        mavlink_msg_global_position_int_decode(&msg, &globalPositionInt);
        emit globalPositionIntUpdated(globalPositionInt);
    break;
    case MAVLINK_MSG_ID_STATUSTEXT:
        mavlink_statustext_t statusText;
        mavlink_msg_statustext_decode(&msg, &statusText);
        emit statusTextUpdated(statusText);
    break;
    case MAVLINK_MSG_ID_PARAM_VALUE:
        mavlink_param_value_t paramValue;
        mavlink_msg_param_value_decode(&msg, &paramValue);
        emit paramValueUpdated(paramValue);
    break;
    case MAVLINK_MSG_ID_PARAM_EXT_VALUE:
        mavlink_param_ext_value_t paramExtValue;
        mavlink_msg_param_ext_value_decode(&msg, &paramExtValue);
        emit paramExtValueUpdated(paramExtValue);
    break;
    case MAVLINK_MSG_ID_LOG_ENTRY:
        mavlink_log_entry_t logEntry;
        mavlink_msg_log_entry_decode(&msg, &logEntry);
        emit logEntryUpdated(logEntry);
    break;
    case MAVLINK_MSG_ID_LOG_DATA:
        mavlink_log_data_t logData;
        mavlink_msg_log_data_decode(&msg, &logData);
        emit logDataUpdated(logData);
    break;
    case MAVLINK_MSG_ID_PARAM_EXT_ACK:
        qDebug() << "got MAVLINK_MSG_ID_PARAM_EXT_ACK";
        mavlink_param_ext_ack_t paramAck;
        mavlink_msg_param_ext_ack_decode(&msg, &paramAck);
        emit paramExtAckUpdated(paramAck);
    break;
    }
}
