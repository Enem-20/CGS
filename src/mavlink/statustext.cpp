#include "statustext.h"

#include <common/mavlink.h>

StatusText::StatusText(QObject *parent)
    : QObject{parent}
{}

void StatusText::handleMavlink(const mavlink_statustext_t& msg) {
    QString severity = "INFO";
    switch(msg.severity) {
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

    emit logUpdated(msg.text, severity);
}
