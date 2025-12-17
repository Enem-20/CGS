#ifndef MAVLINKCONTEXT_H
#define MAVLINKCONTEXT_H

#include <QObject>
#include <QTimer>
#include <QJsonObject>
#include <QSerialPortInfo>
#include <QHash>

#include <common/mavlink.h>

#include "mavlinkpacketizer.h"
#include "mavlinkdevice.h"

class MavlinkContext : public QObject {
    Q_OBJECT

private:
    MavlinkDevice* _activeDevice = nullptr;
    MavlinkDevice* _defaultDevice;
    QHash<QString, MavlinkDevice*> _connectedDevices;
    QTimer _heartBeatTimer;
    mavlink_message_t _heartBeatMsg;
    MavlinkPacketizer_ _packetizer;
    QMetaObject::Connection _parameterListDownloadedConnection;

public:
    MavlinkContext();
    ~MavlinkContext();

    void subscribe(MavlinkSubscriber* subscriber);
    void unsubscribe(MavlinkSubscriber* subscriber);

private:
    void sendHeartbeat();
    void updateMode(uint8_t autopilot, uint8_t type, uint32_t customMode);

signals:
    void messageReceived(const mavlink_message_t& message);

    void parameterListDownloadCompleted();
    void globalPositionIntUpdated(const mavlink_global_position_int_t& msg);
    void heartbeatMessageReceived(const mavlink_message_t& heartbeat);
    void heartbeatUpdated(const mavlink_heartbeat_t& heartbeat);
    void modeUpdated(const QString& mode);
    void armedUpdated(const QString& armed);
    void attitudeUpdated(const QString& rollPitchYaw);
    void speedsUpdated(const QString& speeds);
    void altitudeUpdated(const QString& altitude);
    void logUpdated(const QString& msg, const QString& severity, QColor color);

    void paramUpdated(const mavlink_param_value_t& param);
    void paramExtUpdated(const mavlink_param_ext_value_t& param);
    void logEntryRecieved(const mavlink_log_entry_t& logEntry);
    void logDataRecieved(const mavlink_log_data_t& logData, const mavlink_message_t& msg);
    void paramAckRecieved(const mavlink_param_ext_ack_t& paramAck);

    void deviceConnected(QStringView name, QStringView type);
    void deviceDisconnected(QStringView name);
    void deviceStateChanged(QStringView name, PortState_ state);
    void activeDeviceChanged(QStringView name);

private slots:
    void onMessageReceived(const mavlink_message_t& message);

public slots:
    void onParameterListDownloadCompleted();
    void sendCommand(mavlink_message_t msg);
    void onConnectUDPDevice(quint16 port, const QString& address, QObject *parent);
    void onConnectSerialDevice(QSerialPortInfo portInfo);
    void onConnectSerialDevice(QSerialPortInfo portInfo, int32_t baudRate, uint8_t dataBits, uint8_t stopBits, uint8_t parity, uint8_t flowControl);
    void onMakeDeviceActive(QStringView name);
};

#endif // MAVLINKCONTEXT_H
