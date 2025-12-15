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

    _defaultDevice = new UDPMavlinkDevice(DEFAULT_DEVICE_NAME, 14550, "0.0.0.0");
    _defaultDevice->start();
    _activeDevice = _defaultDevice;
    connect(_activeDevice, &MavlinkDevice::messageReceived, &_packetizer, &MavlinkPacketizer_::onMessageReceived);
    connect(_activeDevice, &MavlinkDevice::messageReceived, this, &MavlinkContext::onMessageReceived);
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

void MavlinkContext::subscribe(MavlinkSubscriber* subscriber) {
    _packetizer.subscribe(subscriber);
}

void MavlinkContext::unsubscribe(MavlinkSubscriber* subscriber) {
    _packetizer.unsubscribe(subscriber);
}

void MavlinkContext::sendHeartbeat() {
    if (!_activeDevice) {
        qWarning() << "Trying to send heartbeat without active device";
        return;
    }

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

void MavlinkContext::onMessageReceived(const mavlink_message_t& msg) {
    // emit messageReceived(msg);
    switch(msg.msgid) {
    case MAVLINK_MSG_ID_HEARTBEAT:
        mavlink_heartbeat_t heartbeat;
        mavlink_msg_heartbeat_decode(&msg, &heartbeat);
        heartbeatMessageReceived(msg);
        heartbeatUpdated(heartbeat);
        updateMode(heartbeat.autopilot, heartbeat.type, heartbeat.custom_mode);
        armedUpdated((heartbeat.base_mode & MAV_MODE_FLAG_SAFETY_ARMED) ? "ARMED" : "DISARMED");
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
    case MAVLINK_MSG_ID_PARAM_EXT_ACK:
        qDebug() << "got MAVLINK_MSG_ID_PARAM_EXT_ACK";
        mavlink_param_ext_ack_t paramAck;
        mavlink_msg_param_ext_ack_decode(&msg, &paramAck);
        emit paramAckRecieved(paramAck);
        break;
    }
}

void MavlinkContext::requestTelemetry() {
    if (!_activeDevice) {
        qWarning() << "Requested telemetry while no device is active";
        return;
    }

    mavlink_message_t msg;

    mavlink_msg_request_data_stream_pack(
        255,
        MAV_COMP_ID_MISSIONPLANNER,
        &msg,
        _activeDevice->getSysId(),
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

void MavlinkContext::sendCommand(mavlink_message_t msg) {
    if (!_activeDevice) {
        qWarning() << "Tying to send command while no device is active";
        return;
    }

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

    connect(device, &MavlinkDevice::portStateChanged, this, [this, device](PortState_ state) {
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

    connect(device, &MavlinkDevice::portStateChanged, this, [this, device](PortState_ state) {
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

    connect(device, &MavlinkDevice::portStateChanged, this, [this, device](PortState_ state) {
        emit deviceStateChanged(device->getName(), state);
    });

    onMakeDeviceActive(device->getName());
}

void MavlinkContext::onMakeDeviceActive(QStringView name) {
    MavlinkDevice* selectedDevice = nullptr;
    if (name == DEFAULT_DEVICE_NAME) {
        selectedDevice = _defaultDevice;
    }
    else if (auto it = _connectedDevices.find(name); it != _connectedDevices.end()) {
        selectedDevice = it.value();
    }
    else {
        qWarning() << "Failed to make device active: " << name;
        return;
    }

    if (selectedDevice == _activeDevice) {
        qDebug() << "Make device active called for active device";
        return;
    }

    disconnect(_activeDevice, &MavlinkDevice::messageReceived, &_packetizer, &MavlinkPacketizer_::onMessageReceived);
    disconnect(_activeDevice, &MavlinkDevice::messageReceived, this, &MavlinkContext::onMessageReceived);
    _activeDevice = selectedDevice;
    connect(_activeDevice, &MavlinkDevice::messageReceived, &_packetizer, &MavlinkPacketizer_::onMessageReceived);
    connect(_activeDevice, &MavlinkDevice::messageReceived, this, &MavlinkContext::onMessageReceived);

    connect(_activeDevice, &MavlinkDevice::portOpened, this, [this](){
        _parameterListDownloadedConnection = connect(this, &MavlinkContext::parameterListDownloadCompleted, this, [this](){
            //onParameterListDownloadCompleted();
            requestTelemetry();
            disconnect(_parameterListDownloadedConnection);
        });
    });

    emit activeDeviceChanged(name);
}
