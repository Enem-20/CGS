#include "mavlinkvehicletelemetry.h"

#include <QDebug>

#include <cmath>
#include <common/mavlink.h>

MavlinkVehicleTelemetry::MavlinkVehicleTelemetry(QObject *parent)
    : VehicleTelemetry{parent}
{}

void MavlinkVehicleTelemetry::onMessage(Message msg) {
    MavlinkMessage* message = msg.read<MavlinkMessage>();
    switch (message->msgid) {
        case MAVLINK_MSG_ID_HEARTBEAT: {
            mavlink_heartbeat_t heartbeat;
            mavlink_msg_heartbeat_decode(message, &heartbeat);
            handleMavlink(heartbeat);
            break;
        }
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
    emit sendMessageRequest(message);
}

void MavlinkVehicleTelemetry::requestStopTelemetry() {
    mavlink_message_t command;
    mavlink_msg_request_data_stream_pack(
        255,
        MAV_COMP_ID_MISSIONPLANNER,
        &command,
        0,
        0,
        MAV_DATA_STREAM_ALL,
        4,
        0
    );

    Message message;
    message.write(&command);
    emit sendMessageRequest(message);
}

void MavlinkVehicleTelemetry::handleMavlink(const mavlink_heartbeat_t& message) {
    bool state = (message.base_mode & MAV_MODE_FLAG_SAFETY_ARMED);  
    if (_armed != state) {
        _armed = state;
        emit armStateUpdated(_armed);
    }

    switch(message.autopilot) {
    case MAV_AUTOPILOT_ARDUPILOTMEGA:
        _mode = message.custom_mode;
        emit modeUpdated(_mode);
        break;
    }
}

void MavlinkVehicleTelemetry::handleMavlink(const mavlink_attitude_t& message) {
    float toDeg = (180.0f / M_PI);
    _orientation = QVector3D(message.roll * toDeg, message.pitch * toDeg, message.yaw * toDeg);
    emit orientationUpdated(_orientation, message.time_boot_ms);
}

void MavlinkVehicleTelemetry::handleMavlink(const mavlink_local_position_ned_t& message) {
    _positionNED.setX(static_cast<float>(message.x));
    _positionNED.setY(static_cast<float>(message.y));
    _positionNED.setZ(static_cast<float>(message.z));
    
    _velocityNED.setX(static_cast<float>(message.vx));
    _velocityNED.setY(static_cast<float>(message.vy));
    _velocityNED.setZ(static_cast<float>(message.vz));

    emit velocityUpdated(_velocityNED, message.time_boot_ms);
}

void MavlinkVehicleTelemetry::handleMavlink(const mavlink_global_position_int_t& message) {
    _altitude = static_cast<float>(message.alt) / 1000.0f;
    _relativeAltitude = static_cast<float>(message.relative_alt);
    _heading = static_cast<float>(message.hdg);
    _latitude = static_cast<float>(message.lat);
    _longitude = static_cast<float>(message.lon);
    _velocityNED.setX(static_cast<float>(message.vx));
    _velocityNED.setY(static_cast<float>(message.vy));
    _velocityNED.setZ(static_cast<float>(message.vz));

    emit altitudeUpdated(_altitude, message.time_boot_ms);
    emit velocityUpdated(_velocityNED, message.time_boot_ms);
}
