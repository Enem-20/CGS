#ifndef MAVLINKCONTEXT_H
#define MAVLINKCONTEXT_H

#include <QObject>
#include <QTimer>
#include <QJsonObject>
#include <QSerialPortInfo>
#include <QHash>

#include <common/mavlink.h>

#include "attitude.h"
#include "localpositionned.h"
#include "globalpositionint.h"
#include "statustext.h"
#include "mavlinkpacketizer.h"
#include <mavlinkdevice.h>

class MavlinkContext : public QObject {
    Q_OBJECT
private:
    MavlinkDevice* _activeDevice = nullptr;
    MavlinkDevice* _defaultDevice;
    QHash<QString, MavlinkDevice*> _connectedDevices;
    Attitude _attitude;
    LocalPositionNED _lPositionNED;
    GlobalPositionInt _globalPositionInt;
    StatusText _statusText;
    QTimer _heartBeatTimer;
    mavlink_message_t _heartBeatMsg;
    QJsonObject _existingModes;
    MavlinkPacketizer _packetizer;
    QMetaObject::Connection _parameterListDownloadedConnection;
public:
    MavlinkContext();
    ~MavlinkContext();
    void sendHeartbeat();

private:
    void updateMode(uint8_t autopilot, uint8_t type, uint32_t customMode);

signals:
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
    void deviceStateChanged(QStringView name, PortState state);
    void activeDeviceChanged(QStringView name);

private slots:
    void handleMavlinkMessage(mavlink_message_t msg);
    void requestTelemetry();
public slots:
    void onParameterListDownloadCompleted();
    void loadModes();
    void sendCommand(const mavlink_message_t& msg);
    void onConnectUDPDevice(quint16 port, const QString& address, QObject *parent);
    void onConnectSerialDevice(QSerialPortInfo portInfo);
    void onConnectSerialDevice(QSerialPortInfo portInfo, int32_t baudRate, uint8_t dataBits, uint8_t stopBits, uint8_t parity, uint8_t flowControl);
    void onMakeDeviceActive(QStringView name);
};

#endif // MAVLINKCONTEXT_H
