#include "mavlinkcontext.h"

#include <QFile>
#include <QColor>
#include <QJsonDocument>

#include "udpmavlinkdevice.h"
#include "serialmavlinkdevice.h"

MavlinkContext::MavlinkContext()
    : _activeDevice(new UDPMavlinkDevice(14550, "0.0.0.0", this))
    , _heartBeatTimer(this)
{
    _heartBeatTimer.start(1000);
    connect(&_heartBeatTimer, &QTimer::timeout, this, [this](){
        //if (_activeDevice.getSocketThread() != );
    });

    connect(&_attitude, &Attitude::orientationUpdated, this, [this](const QVector3D& orientation) {
        QString result("R: %1°, P: %2°, Y: %3°");
        emit attitudeUpdated(result
                                 .arg(QString::asprintf("%.2f", orientation.x()))
                                 .arg(QString::asprintf("%.2f", orientation.y()))
                                 .arg(QString::asprintf("%.2f", orientation.z())));
    });

    connect(&_lPositionNED, &LocalPositionNED::speedsUpdated, this, [this](const QVector3D speeds) {
        QString result("FS: %1 m/s, LS: %2 m/s, VS: %3 m/s");
        emit speedsUpdated(result
                               .arg(QString::asprintf("%.2f", speeds.x()))
                               .arg(QString::asprintf("%.2f", speeds.y()))
                               .arg(QString::asprintf("%.2f", speeds.z()))
                           );
    });

    connect(&_globalPositionInt, &GlobalPositionInt::altitudeUpdated, this, [this](int32_t altitude) {
        QString result("Alt: %1 m");
        emit altitudeUpdated(result.arg(altitude));
    });

    connect(&_statusText, &StatusText::logUpdated, this, [this](QString msg, QString severity) {
        QColor resultColor;
        if(severity == "EMERGENCY") {
            resultColor = Qt::red;
        }
        else if(severity == "ALERT") {
            resultColor = Qt::red;
        }
        else if(severity == "CRITICAL") {
            resultColor = Qt::red;
        }
        else if (severity == "ERROR") {
            resultColor = Qt::red;
        }
        else if (severity == "WARNING") {
            resultColor = Qt::yellow;
        }
        else if (severity == "NOTICE") {
            resultColor = Qt::green;
        }
        else if (severity == "INFO") {
            resultColor = Qt::black;
        }
        else if (severity == "DEBUG") {
            resultColor = Qt::blue;
        }
        emit logUpdated(msg, severity, resultColor);
    });

    if (UDPMavlinkDevice* udpDevice = dynamic_cast<UDPMavlinkDevice*>(_activeDevice)) {
        connect(udpDevice, &UDPMavlinkDevice::messageReceived, this, &MavlinkContext::handleMavlinkMessage);
        //connect(&udpDevice, &UDPMavlinkDevice::messageReceived, &_packetizer, &MavlinkPacketizer::mavlinkMsgReceived);
    }
}

MavlinkContext::~MavlinkContext() {
    delete _activeDevice;
}

void MavlinkContext::sendHeartbeat() {
    mavlink_msg_heartbeat_pack(
        255,
        MAV_COMP_ID_MISSIONPLANNER,
        &_heartBeatMsg,
        MAV_TYPE_GCS,
        MAV_AUTOPILOT_INVALID,
        0,
        0,
        MAV_STATE_ACTIVE
        );
    _activeDevice->sendCommand(_heartBeatMsg);
}

void MavlinkContext::updateMode(uint8_t autopilot, uint8_t type, uint32_t customMode) {
    switch(autopilot) {
    case MAV_AUTOPILOT_ARDUPILOTMEGA:
        QString stringifiedCustomMode = QString::number(customMode);
        if(_existingModes.contains(stringifiedCustomMode))
            emit modeUpdated(_existingModes[stringifiedCustomMode].toString());
        else
            emit modeUpdated("UNKNOWN");
        break;
    }
}

void MavlinkContext::handleMavlinkMessage(mavlink_message_t msg) {
    switch(msg.msgid) {
    case MAVLINK_MSG_ID_HEARTBEAT:
        mavlink_heartbeat_t heartbeat;
        mavlink_msg_heartbeat_decode(&msg, &heartbeat);
        heartbeatMessageReceived(msg);
        heartbeatUpdated(heartbeat);
        updateMode(heartbeat.autopilot, heartbeat.type, heartbeat.custom_mode);
        armedUpdated((heartbeat.base_mode & MAV_MODE_FLAG_SAFETY_ARMED) ? "ARMED" : "DISARMED");
        break;
    case MAVLINK_MSG_ID_ATTITUDE:
        mavlink_attitude_t attitude;
        mavlink_msg_attitude_decode(&msg, &attitude);
        _attitude.handleMavlink(attitude);
        break;
    case MAVLINK_MSG_ID_LOCAL_POSITION_NED:
        mavlink_local_position_ned_t localPositionNED;
        mavlink_msg_local_position_ned_decode(&msg, &localPositionNED);
        _lPositionNED.handleMavlink(localPositionNED);
        break;
    case MAVLINK_MSG_ID_GLOBAL_POSITION_INT:
        mavlink_global_position_int_t globalPositionInt;
        mavlink_msg_global_position_int_decode(&msg, &globalPositionInt);
        _globalPositionInt.handleMavlink(globalPositionInt);
        emit globalPositionIntUpdated(globalPositionInt);
        break;
    case MAVLINK_MSG_ID_STATUSTEXT:
        mavlink_statustext_t statusText;
        mavlink_msg_statustext_decode(&msg, &statusText);
        _statusText.handleMavlink(statusText);
        break;
    case MAVLINK_MSG_ID_PARAM_VALUE:
        mavlink_param_value_t paramValue;
        mavlink_msg_param_value_decode(&msg, &paramValue);
        emit paramUpdated(paramValue);
        break;
    case MAVLINK_MSG_ID_PARAM_EXT_VALUE:
        mavlink_param_ext_value_t paramExtValue;
        mavlink_msg_param_ext_value_decode(&msg, &paramExtValue);
        emit paramExtUpdated(paramExtValue);
        break;
    case MAVLINK_MSG_ID_LOG_ENTRY:
        mavlink_log_entry_t logEntry;
        mavlink_msg_log_entry_decode(&msg, &logEntry);
        emit logEntryRecieved(logEntry);
        break;
    case MAVLINK_MSG_ID_LOG_DATA:
        mavlink_log_data_t logData;
        mavlink_msg_log_data_decode(&msg, &logData);
        emit logDataRecieved(logData, msg);
        break;
    case MAVLINK_MSG_ID_PARAM_EXT_ACK:
        qDebug() << "got MAVLINK_MSG_ID_PARAM_EXT_ACK";
        mavlink_param_ext_ack_t paramAck;
        mavlink_msg_param_ext_ack_decode(&msg, &paramAck);
        emit paramAckRecieved(paramAck);
        break;
    }
}

void MavlinkContext::loadModes() {
    QFile file("modes.json");
    if(!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Unable to open file: " << "modes.json";
        return;
    }
    QByteArray data = file.readAll();
    file.close();
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(data, &error);
    if(error.error != QJsonParseError::NoError) {
        qWarning() << "modes.json parse error: " << error.errorString();
        return;
    }

    _existingModes = doc.object();
}

void MavlinkContext::sendCommand(const mavlink_message_t& msg) {
    _activeDevice->sendCommand(msg);
}

void MavlinkContext::onConnectSerialDevice(QSerialPortInfo portInfo) {
    disconnect(_activeDevice, &UDPMavlinkDevice::messageReceived, this, &MavlinkContext::handleMavlinkMessage);
    delete _activeDevice;
    _activeDevice = new SerialMavlinkDevice(portInfo, this);
    connect(_activeDevice, &SerialMavlinkDevice::messageReceived, this, &MavlinkContext::handleMavlinkMessage);

}
