#include "statustext.h"

#include <QDebug>

#include <common/mavlink.h>

StatusText::StatusText(QObject *parent)
    : MavlinkSubscriber{parent}
{}

QVector<uint32_t> StatusText::getSubscribtionMessageIds() const {
    return { MAVLINK_MSG_ID_STATUSTEXT };
}

void StatusText::onMessageReceived(const mavlink_message_t& message) {
    switch (message.msgid) {
    case MAVLINK_MSG_ID_STATUSTEXT: {
        mavlink_statustext_t statusText;
        mavlink_msg_statustext_decode(&message, &statusText);
        handleMavlink(statusText);
        break;
    }
    default:
        qWarning() << "StatusText: Wrong message received: " << message.msgid;
    }
}


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
