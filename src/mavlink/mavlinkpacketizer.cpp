#include "mavlinkpacketizer.h"

#include <common/mavlink.h>

#include <QDebug>

#include "mavlinksubscriber.h"

MavlinkPacketizer::MavlinkPacketizer(QObject *parent)
    : QObject{parent} {}

void MavlinkPacketizer::subscribe(MavlinkSubscriber* subscriber) {
    int32_t subscriberIndex = _subscribers.indexOf(subscriber);
    if (subscriberIndex != -1) {
        qWarning() << "MavlinkPacketizer: subscriber is already subscribed";
        return;
    }

    connect(subscriber, &MavlinkSubscriber::sendMessage, this, &MavlinkPacketizer::onMessageSendRequest);
    _subscribers.push_back(subscriber);

    QVector<uint32_t> messages = subscriber->getSubscribtionMessageIds();
    for (uint32_t messageId : messages) {
        if (!_subscriptions.contains(messageId)) {
            _subscriptions.insert(messageId, {});
        }
        _subscriptions[messageId].push_back(subscriber);
    }
}

void MavlinkPacketizer::unsubscribe(MavlinkSubscriber* subscriber) {
    int32_t subscriberIndex = _subscribers.indexOf(subscriber);
    if (subscriberIndex == -1) {
        return;
    }

    disconnect(subscriber, &MavlinkSubscriber::sendMessage, this, &MavlinkPacketizer::onMessageSendRequest);
    _subscribers.push_back(subscriber);

    QVector<uint32_t> messages = subscriber->getSubscribtionMessageIds();
    for (uint32_t messageId : messages) {
        if (!_subscriptions.contains(messageId)) {
            _subscriptions.insert(messageId, {});
        }

        int32_t index = _subscriptions[messageId].indexOf(subscriber);
        if (index != -1) {
            _subscriptions[messageId].erase(_subscriptions[messageId].begin() + index);
        }
    }
}

void MavlinkPacketizer::onMessageReceived(const mavlink_message_t& message) {
    if (!_subscriptions.contains(message.msgid)) {
        return;
    }

    for (MavlinkSubscriber* subscriber : _subscriptions[message.msgid]) {
        QMetaObject::invokeMethod(subscriber, "onMessageReceived", Q_ARG(const mavlink_message_t&, message));
    }

    // emit mavlinkMessageUpdated(msg);
    // switch(msg.msgid) {
    // case MAVLINK_MSG_ID_HEARTBEAT:
    //     mavlink_heartbeat_t heartbeat;
    //     mavlink_msg_heartbeat_decode(&msg, &heartbeat);
    //     emit heartbeatUpdated(heartbeat);
    // break;
    // case MAVLINK_MSG_ID_ATTITUDE:
    //     mavlink_attitude_t attitude;
    //     mavlink_msg_attitude_decode(&msg, &attitude);
    //     emit attitudeUpdated(attitude);
    // break;
    // case MAVLINK_MSG_ID_LOCAL_POSITION_NED:
    //     mavlink_local_position_ned_t localPositionNED;
    //     mavlink_msg_local_position_ned_decode(&msg, &localPositionNED);
    //     emit localPositionNEDUpdated(localPositionNED);
    // break;
    // case MAVLINK_MSG_ID_GLOBAL_POSITION_INT:
    //     mavlink_global_position_int_t globalPositionInt;
    //     mavlink_msg_global_position_int_decode(&msg, &globalPositionInt);
    //     emit globalPositionIntUpdated(globalPositionInt);
    // break;
    // case MAVLINK_MSG_ID_STATUSTEXT:
    //     mavlink_statustext_t statusText;
    //     mavlink_msg_statustext_decode(&msg, &statusText);
    //     emit statusTextUpdated(statusText);
    // break;
    // case MAVLINK_MSG_ID_PARAM_VALUE:
    //     mavlink_param_value_t paramValue;
    //     mavlink_msg_param_value_decode(&msg, &paramValue);
    //     emit paramValueUpdated(paramValue);
    // break;
    // case MAVLINK_MSG_ID_PARAM_EXT_VALUE:
    //     mavlink_param_ext_value_t paramExtValue;
    //     mavlink_msg_param_ext_value_decode(&msg, &paramExtValue);
    //     emit paramExtValueUpdated(paramExtValue);
    // break;
    // case MAVLINK_MSG_ID_LOG_ENTRY:
    //     mavlink_log_entry_t logEntry;
    //     mavlink_msg_log_entry_decode(&msg, &logEntry);
    //     emit logEntryUpdated(logEntry);
    // break;
    // case MAVLINK_MSG_ID_LOG_DATA:
    //     mavlink_log_data_t logData;
    //     mavlink_msg_log_data_decode(&msg, &logData);
    //     emit logDataUpdated(logData);
    // break;
    // case MAVLINK_MSG_ID_PARAM_EXT_ACK:
    //     qDebug() << "got MAVLINK_MSG_ID_PARAM_EXT_ACK";
    //     mavlink_param_ext_ack_t paramAck;
    //     mavlink_msg_param_ext_ack_decode(&msg, &paramAck);
    //     emit paramExtAckUpdated(paramAck);
    // break;
    // }
}

void MavlinkPacketizer::onMessageSendRequest(const mavlink_message_t& message) {
    emit requestMessageSend(message);
}
