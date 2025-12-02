#include "telemetry.h"

Telemetry::Telemetry(QObject *parent)
    : QObject{parent}
{

}

void Telemetry::handleMavlink(const mavlink_message_t& msg) {

}
