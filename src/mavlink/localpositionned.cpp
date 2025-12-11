#include "localpositionned.h"

#include <common/mavlink.h>

LocalPositionNED::LocalPositionNED(QObject *parent)
    : QObject{parent}
{}

void LocalPositionNED::handleMavlink(const mavlink_local_position_ned_t& msg) {
    speedsUpdated(_speeds = QVector3D(msg.vx, msg.vy, msg.vz));
}
