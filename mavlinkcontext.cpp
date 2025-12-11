#include "mavlinkcontext.h"

#include <QFile>
#include <QColor>
#include <QJsonDocument>

#include "mavlinkdevice.h"
#include "udpmavlinkdevice.h"
#include "serialmavlinkdevice.h"

MavlinkContext::MavlinkContext()
    : _heartBeatTimer(this)
{
    _heartBeatTimer.start(1000);
    connect(&_heartBeatTimer, &QTimer::timeout, this, &MavlinkContext::sendHeartbeat);

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

    _defaultDevice = new UDPMavlinkDevice(DEFAULT_DEVICE_NAME, 14550, "0.0.0.0");
    _defaultDevice->start();
    _activeDevice = _defaultDevice;
    connect(_activeDevice, &MavlinkDevice::messageReceived, this, &MavlinkContext::handleMavlinkMessage);
    QTimer::singleShot(0, this, [this] {
        emit activeDeviceChanged(DEFAULT_DEVICE_NAME);
    });
}

MavlinkContext::~MavlinkContext() {
    _defaultDevice->deleteLater();
    for (MavlinkDevice* device : _connectedDevices.values()) {
        device->deleteLater();
    }
}

void MavlinkContext::sendHeartbeat() {
    if (!_activeDevice) return;
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

    sendCommand(_heartBeatMsg);
}

void MavlinkContext::updateMode(uint8_t autopilot, uint8_t type, uint32_t customMode) {
    switch(autopilot) {
    case MAV_AUTOPILOT_ARDUPILOTMEGA:
        QString stringifiedCustomMode = QString::number(customMode);
        if (_existingModes.contains(stringifiedCustomMode))
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

void MavlinkContext::requestTelemetry() {
    if (!_activeDevice) return;

    mavlink_message_t msg;

    mavlink_msg_request_data_stream_pack(
        255,
        MAV_COMP_ID_MISSIONPLANNER,
        &msg,
        1,
        0,
        MAV_DATA_STREAM_ALL,
        4,
        1
    );

    sendCommand(msg);
}

void MavlinkContext::onParameterListDownloadCompleted() {
    emit parameterListDownloadCompleted();
}

void MavlinkContext::loadModes() {
    QFile file("modes.json");
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Unable to open file: " << "modes.json";
        return;
    }
    QByteArray data = file.readAll();
    file.close();
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(data, &error);
    if (error.error != QJsonParseError::NoError) {
        qWarning() << "modes.json parse error: " << error.errorString();
        return;
    }

    _existingModes = doc.object();
}

void MavlinkContext::sendCommand(const mavlink_message_t& msg) {
    if (!_activeDevice) return;

    QMetaObject::invokeMethod(_activeDevice, "sendCommand", Qt::QueuedConnection,
                              Q_ARG(const mavlink_message_t&, msg));
    //_activeDevice->sendCommand(msg);
}

void MavlinkContext::onConnectUDPDevice(quint16 port, const QString& address, QObject *parent) {
    const QString name = address + ":" + QString::number(port);
    if (_connectedDevices.contains(name)) {
        return;
    }

    UDPMavlinkDevice* device = new UDPMavlinkDevice(name, port, address);
    device->start();
    _connectedDevices.insert(device->getName().toString(), device);
    emit deviceConnected(device->getName(), device->getType());

    connect(device, &MavlinkDevice::portStateChanged, this, [this, device](PortState state) {
        emit deviceStateChanged(device->getName(), state);
    });
}

void MavlinkContext::onConnectSerialDevice(QSerialPortInfo portInfo) {
    const QString name = portInfo.portName();
    if (_connectedDevices.contains(name)) {
        return;
    }

    SerialMavlinkDevice* device = new SerialMavlinkDevice(name, portInfo);
    device->start();
    _connectedDevices.insert(device->getName().toString(), device);
    emit deviceConnected(device->getName(), device->getType());

    connect(device, &MavlinkDevice::portStateChanged, this, [this, device](PortState state) {
        emit deviceStateChanged(device->getName(), state);
    });

    onMakeDeviceActive(device->getName());
}

void MavlinkContext::onConnectSerialDevice(QSerialPortInfo portInfo, int32_t baudRate, uint8_t dataBits, uint8_t stopBits, uint8_t parity, uint8_t flowControl) {
    const QString name = portInfo.portName();
    if (_connectedDevices.contains(name)) {
        return;
    }

    SerialMavlinkDevice* device = new SerialMavlinkDevice(name, portInfo);
    device->start();
    device->setupPort(portInfo, baudRate, dataBits, stopBits, parity, flowControl);
    _connectedDevices.insert(device->getName().toString(), device);
    emit deviceConnected(device->getName(), device->getType());

    connect(device, &MavlinkDevice::portStateChanged, this, [this, device](PortState state) {
        emit deviceStateChanged(device->getName(), state);
    });

    onMakeDeviceActive(device->getName());
}

void MavlinkContext::onMakeDeviceActive(QStringView name) {
    if (name == DEFAULT_DEVICE_NAME) {
        disconnect(_activeDevice, &MavlinkDevice::messageReceived, this, &MavlinkContext::handleMavlinkMessage);
        _activeDevice = _defaultDevice;
        connect(_activeDevice, &MavlinkDevice::messageReceived, this, &MavlinkContext::handleMavlinkMessage);
        connect(_activeDevice, &MavlinkDevice::portOpened, this, [this](){
            _parameterListDownloadedConnection = connect(this, &MavlinkContext::parameterListDownloadCompleted, this, [this](){
                //onParameterListDownloadCompleted();
                requestTelemetry();
                disconnect(_parameterListDownloadedConnection);
            });
        });
        emit activeDeviceChanged(name);
        return;
    }

    if (auto it = _connectedDevices.find(name); it != _connectedDevices.end()) {
        MavlinkDevice* device = it.value();
        if (device == _activeDevice) return;
        disconnect(_activeDevice, &MavlinkDevice::messageReceived, this, &MavlinkContext::handleMavlinkMessage);
        _activeDevice = device;
        connect(_activeDevice, &MavlinkDevice::messageReceived, this, &MavlinkContext::handleMavlinkMessage);

        connect(_activeDevice, &MavlinkDevice::portOpened, this, [this](){
            _parameterListDownloadedConnection = connect(this, &MavlinkContext::parameterListDownloadCompleted, this, [this](){
                //onParameterListDownloadCompleted();
                requestTelemetry();
                disconnect(_parameterListDownloadedConnection);
            });
        });
        emit activeDeviceChanged(name);
        return;
    }

    qDebug() << "Failed to make device active: " << name;
    //connect(&udpDevice, &UDPMavlinkDevice::messageReceived, &_packetizer, &MavlinkPacketizer::mavlinkMsgReceived);
}
