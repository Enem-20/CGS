#include "mavlinkvehiclestatuslog.h"

#include "common/mavlink.h"

MavlinkVehicleStatusLog::MavlinkVehicleStatusLog(QObject *parent) noexcept
    : VehicleStatusLog(parent) {}

void MavlinkVehicleStatusLog::onMessage(Message msg) {
    MavlinkMessage* message = msg.read<MavlinkMessage>();
    switch (message->msgid) {
        case MAVLINK_MSG_ID_STATUSTEXT: {
            mavlink_statustext_t statusText;
            mavlink_msg_statustext_decode(message, &statusText);
            handleMavlink(statusText);
            break;
        }
    }
}

void MavlinkVehicleStatusLog::handleMavlink(mavlink_statustext_t message) {
    QString severity = "INFO";
    switch(message.severity) {
    case MAV_SEVERITY_EMERGENCY:
        severity = "EMERGENCY";
        break;
    case MAV_SEVERITY_ALERT:
        severity = "ALERT";
        break;
    case MAV_SEVERITY_CRITICAL:
        severity = "CRITICAL";
        break;
    case MAV_SEVERITY_ERROR:
        severity = "ERROR";
        break;
    case MAV_SEVERITY_WARNING:
        severity = "WARNING";
        break;
    case MAV_SEVERITY_NOTICE:
        severity = "NOTICE";
        break;
    case MAV_SEVERITY_INFO:
        severity = "INFO";
        break;
    case MAV_SEVERITY_DEBUG:
        severity = "DEBUG";
        break;
    }

    const uint32_t MAX_TEXT_LENGTH = 50;
    char textBuffer[MAX_TEXT_LENGTH + 1]{'\0'};
    memcpy(textBuffer, message.text, MAX_TEXT_LENGTH);
    QString text(textBuffer);

    emit statusMessageReceived(text, severity);
}
