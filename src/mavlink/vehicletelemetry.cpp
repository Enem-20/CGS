#include "vehicletelemetry.h"

#include <QDebug>

#include <cmath>
#include <common/mavlink.h>

VehicleTelemetry::VehicleTelemetry(QObject *parent)
    : MavlinkSubscriber{parent}
{}

QVector<uint32_t> VehicleTelemetry::getSubscribtionMessageIds() const {
    return {
        MAVLINK_MSG_ID_ATTITUDE,
        MAVLINK_MSG_ID_LOCAL_POSITION_NED,
        MAVLINK_MSG_ID_GLOBAL_POSITION_INT
    };
}

void VehicleTelemetry::onMessageReceived(const mavlink_message_t& message) {
    switch (message.msgid) {
    case MAVLINK_MSG_ID_ATTITUDE: {
        mavlink_attitude_t attitude;
        mavlink_msg_attitude_decode(&message, &attitude);
        handleMavlink(attitude);
        break;
    }
    case MAVLINK_MSG_ID_LOCAL_POSITION_NED: {
        mavlink_local_position_ned_t localPositionNED;
        mavlink_msg_local_position_ned_decode(&message, &localPositionNED);
        handleMavlink(localPositionNED);
        break;
    }
    case MAVLINK_MSG_ID_GLOBAL_POSITION_INT: {
        mavlink_global_position_int_t globalPositionInt;
        mavlink_msg_global_position_int_decode(&message, &globalPositionInt);
        handleMavlink(globalPositionInt);
        break;
    }
    default:
        qWarning() << "VehicleTelemetry: Wrong message received: " << message.msgid;
    }
}

void VehicleTelemetry::handleMavlink(const mavlink_attitude_t& message) {
    float toDeg = (180.0f / M_PI);
    _orientation = QVector3D(message.roll * toDeg, message.pitch * toDeg, message.yaw * toDeg);
    emit orientationUpdated(_orientation);
}

void VehicleTelemetry::handleMavlink(const mavlink_local_position_ned_t& message) {
    _speeds = QVector3D(message.vx, message.vy, message.vz);
    emit speedsUpdated(_speeds);
}

void VehicleTelemetry::handleMavlink(const mavlink_global_position_int_t& message) {
    _altitude =  message.relative_alt / 1000;
    emit altitudeUpdated(_altitude);
}
