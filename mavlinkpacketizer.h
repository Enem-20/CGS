#ifndef PACKETIZER_H
#define PACKETIZER_H

#include <QObject>

struct __mavlink_global_position_int_t;
typedef __mavlink_global_position_int_t mavlink_global_position_int_t;

struct __mavlink_local_position_ned_t;
typedef __mavlink_local_position_ned_t mavlink_local_position_ned_t;

struct __mavlink_attitude_t;
typedef __mavlink_attitude_t mavlink_attitude_t;

struct __mavlink_statustext_t;
typedef __mavlink_statustext_t mavlink_statustext_t;

struct __mavlink_message;
typedef __mavlink_message mavlink_message_t;

struct __mavlink_heartbeat_t;
typedef __mavlink_heartbeat_t mavlink_heartbeat_t;

struct __mavlink_param_value_t;
typedef __mavlink_param_value_t mavlink_param_value_t;

struct __mavlink_param_ext_value_t;
typedef __mavlink_param_ext_value_t mavlink_param_ext_value_t;

struct __mavlink_param_ext_ack_t;
typedef __mavlink_param_ext_ack_t mavlink_param_ext_ack_t;

struct __mavlink_log_entry_t;
typedef __mavlink_log_entry_t mavlink_log_entry_t;

struct __mavlink_log_data_t;
typedef __mavlink_log_data_t mavlink_log_data_t;

class MavlinkPacketizer : public QObject
{
    Q_OBJECT
public:
    explicit MavlinkPacketizer(QObject *parent = nullptr);

signals:
    void mavlinkMessageUpdated(const mavlink_message_t& msg);

    void globalPositionIntUpdated(const mavlink_global_position_int_t& msg);
    void localPositionNEDUpdated(const mavlink_local_position_ned_t& msg);
    void attitudeUpdated(const mavlink_attitude_t& msg);
    void statusTextUpdated(const mavlink_statustext_t& msg);
    void heartbeatUpdated(const mavlink_heartbeat_t& msg);
    void paramValueUpdated(const mavlink_param_value_t& msg);
    void paramExtValueUpdated(const mavlink_param_ext_value_t& msg);
    void paramExtAckUpdated(const mavlink_param_ext_ack_t& msg);
    void logEntryUpdated(const mavlink_log_entry_t& msg);
    void logDataUpdated(const mavlink_log_data_t& msg);
public slots:
    void mavlinkMsgReceived(const mavlink_message_t& msg);
};

#endif // PACKETIZER_H
