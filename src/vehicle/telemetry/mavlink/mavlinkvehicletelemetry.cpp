#include "mavlinkvehicletelemetry.h"

#include <QDebug>

#include <cmath>
#include <common/mavlink.h>

MavlinkVehicleTelemetry::MavlinkVehicleTelemetry(QObject *parent)
    : VehicleTelemetry{parent}
{}

void MavlinkVehicleTelemetry::onMessageReceived(Message msg) {
    MavlinkMessage* message = msg.read<MavlinkMessage>();
    switch (message->msgid) {
        case MAVLINK_MSG_ID_ATTITUDE: {
            mavlink_attitude_t attitude;
            mavlink_msg_attitude_decode(message, &attitude);
            handleMavlink(attitude);
            break;
        }
        case MAVLINK_MSG_ID_LOCAL_POSITION_NED: {
            mavlink_local_position_ned_t localPositionNED;
            mavlink_msg_local_position_ned_decode(message, &localPositionNED);
            handleMavlink(localPositionNED);
            break;
        }
        case MAVLINK_MSG_ID_GLOBAL_POSITION_INT: {
            mavlink_global_position_int_t globalPositionInt;
            mavlink_msg_global_position_int_decode(message, &globalPositionInt);
            handleMavlink(globalPositionInt);
            break;
        }
    }
}

void MavlinkVehicleTelemetry::requestTelemetry() {
    mavlink_message_t command;
    mavlink_msg_request_data_stream_pack(
        255,
        MAV_COMP_ID_MISSIONPLANNER,
        &command,
        0,
        0,
        MAV_DATA_STREAM_ALL,
        4,
        1
    );

    Message message;
    message.write(&command);
    emit sendMessage(message);
}

void MavlinkVehicleTelemetry::handleMavlink(const mavlink_attitude_t& message) {
    float toDeg = (180.0f / M_PI);
    _orientation = QVector3D(message.roll * toDeg, message.pitch * toDeg, message.yaw * toDeg);
    emit orientationUpdated(_orientation);
}

void MavlinkVehicleTelemetry::handleMavlink(const mavlink_local_position_ned_t& message) {
    _speeds = QVector3D(message.vx, message.vy, message.vz);
    emit speedsUpdated(_speeds);
}

void MavlinkVehicleTelemetry::handleMavlink(const mavlink_global_position_int_t& message) {
    _altitude =  message.relative_alt / 1000;
    emit altitudeUpdated(_altitude);
}
