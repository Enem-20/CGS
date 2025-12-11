#include "attitude.h"

#include <common/mavlink.h>
#include <cmath>

Attitude::Attitude(QObject *parent)
    : QObject{parent}
{}

void Attitude::handleMavlink(const mavlink_attitude_t& msg) {
    float toDeg = (180.0f / M_PI);
    emit orientationUpdated(_orientation = QVector3D(msg.roll * toDeg, msg.pitch * toDeg, msg.yaw * toDeg));
}
