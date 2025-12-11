#include "globalpositionint.h"

#include <common/mavlink.h>

GlobalPositionInt::GlobalPositionInt(QObject *parent)
    : QObject{parent}
{

}

void GlobalPositionInt::handleMavlink(const mavlink_global_position_int_t& msg) {
    emit altitudeUpdated(msg.relative_alt / 1000);
}
