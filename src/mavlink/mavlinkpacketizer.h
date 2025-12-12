#ifndef PACKETIZER_H
#define PACKETIZER_H

#include <QObject>

#include <QHash>
#include <QVector>

#include "mavlinktypes.h"

class MavlinkSubscriber;

class MavlinkPacketizer : public QObject {
    Q_OBJECT

public:
    explicit MavlinkPacketizer(QObject *parent = nullptr);

    void subscribe(MavlinkSubscriber* subscriber);
    void unsubscribe(MavlinkSubscriber* subscriber);

private:
    QVector<MavlinkSubscriber*> _subscribers;
    QHash<uint32_t, QVector<MavlinkSubscriber*>> _subscriptions;

// signals:
//     void mavlinkMessageUpdated(const mavlink_message_t& msg);

//     void globalPositionIntUpdated(const mavlink_global_position_int_t& msg);
//     void localPositionNEDUpdated(const mavlink_local_position_ned_t& msg);
//     void attitudeUpdated(const mavlink_attitude_t& msg);
//     void statusTextUpdated(const mavlink_statustext_t& msg);
//     void heartbeatUpdated(const mavlink_heartbeat_t& msg);
//     void paramValueUpdated(const mavlink_param_value_t& msg);
//     void paramExtValueUpdated(const mavlink_param_ext_value_t& msg);
//     void paramExtAckUpdated(const mavlink_param_ext_ack_t& msg);
//     void logEntryUpdated(const mavlink_log_entry_t& msg);
//     void logDataUpdated(const mavlink_log_data_t& msg);

public slots:
    void onMessageReceived(const mavlink_message_t& message);
    void onMessageSendRequest(const mavlink_message_t& message);

signals:
    void requestMessageSend(const mavlink_message_t& message);
};

#endif // PACKETIZER_H
