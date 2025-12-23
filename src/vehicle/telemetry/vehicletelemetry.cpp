#include "vehicletelemetry.h"

#include <QDebug>

#include <cmath>
#include <common/mavlink.h>

VehicleTelemetry::VehicleTelemetry(QObject *parent)
    : ProtocolSubscriber{parent}
{}

void VehicleTelemetry::onStartTelemetry(){
    requestTelemetry();
}

void VehicleTelemetry::onStopTelemetry() {
    requestStopTelemetry();
}

void VehicleTelemetry::onMessage(Message msg) {
    mavlink_message_t* converted = msg.read<mavlink_message_t>();
    switch (converted->msgid) {
    case MAVLINK_MSG_ID_GLOBAL_POSITION_INT:
        mavlink_global_position_int_t globalPos;
        mavlink_msg_global_position_int_decode(converted, &globalPos);
        emit velocityUpdated(QVector3D(globalPos.vx, globalPos.vy, globalPos.vz), globalPos.time_boot_ms);
    break;
    case MAVLINK_MSG_ID_ATTITUDE:
        mavlink_attitude_t attitude;
        mavlink_msg_attitude_decode(converted, &attitude);
        emit orientationUpdated(QVector3D(attitude.roll, attitude.pitch, attitude.yaw), attitude.time_boot_ms);
    break;
    case MAVLINK_MSG_ID_ALTITUDE:
        mavlink_altitude_t altitude;
        mavlink_msg_altitude_decode(converted, &altitude);
        emit altitudeUpdated(altitude.altitude_relative, altitude.time_usec * 1000);
    break;
    }
}
