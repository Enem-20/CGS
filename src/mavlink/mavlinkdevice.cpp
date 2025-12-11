#include "mavlinkdevice.h"

#include <QIODevice>
#include <common/mavlink.h>

MavlinkDevice::MavlinkDevice(const QString& name, const QString& type, QIODevice* device, QObject *parent)
    : QThread(parent)
    , _queueSendTimer(parent)
    , _connectionWatchdog(parent)
    , _waitPacketTimer(parent)
    , _name(name)
    , _type(type)
{
    _device = device;
    _mavlinkStatus = new mavlink_status_t;
    //if(_device)
    //    _device->open(QIODevice::ReadWrite);

    //_queueSendTimer.start(500);
    connect(&_queueSendTimer, &QTimer::timeout, this, [this](){
        if(_device && _device->isOpen()) {
            while(!_messageQueue.empty()) {
                _device->write(_messageQueue.front());
                _messageQueue.pop();
            }
        }
    });
    //_connectionWatchdog.start(500);
    connect(&_connectionWatchdog, &QTimer::timeout, this, [this](){
        if(_device)
            if(!_device->isOpen())
                _device->open(QIODevice::ReadWrite);
    });

    connect(&_waitPacketTimer, &QTimer::timeout, this, &MavlinkDevice::waitPacketTimeout);
    connect(_device, &QIODevice::readyRead, this, &MavlinkDevice::readBytes);
}

MavlinkDevice::~MavlinkDevice() {
    delete _mavlinkStatus;
}

uint8_t MavlinkDevice::getSysId() const {
    return _sysid;
}

uint8_t MavlinkDevice::getCompId() const {
    return _compid;
}

QStringView MavlinkDevice::getName() const {
    return _name;
}

QStringView MavlinkDevice::getType() const {
    return _type;
}

void MavlinkDevice::readBytes() {
    _waitPacketTimer.stop();

    _inputBuffer += _device->readAll();

    if (_inputBuffer.isEmpty()) {
        return;
    }

    mavlink_message_t msg;
    bool messageParsed = false;
    size_t processedBytes = 0;

    for (size_t i = 0; i < _inputBuffer.size(); ++i) {
        uint8_t byte = static_cast<uint8_t>(_inputBuffer[i]);

        if (mavlink_parse_char(MAVLINK_COMM_0, byte, &msg, _mavlinkStatus)) {
            emit messageReceived(msg);
            messageParsed = true;
            processedBytes = i + 1;
        }
    }

    if (processedBytes > 0) {
        _inputBuffer.remove(0, processedBytes);
    }

    if (!_inputBuffer.isEmpty() && !messageParsed) {
        _waitPacketTimer.start(200);
    }
}

void MavlinkDevice::waitPacketTimeout() {
    _inputBuffer.clear();
}

void MavlinkDevice::sendCommand(const mavlink_message_t& command) {
    uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
    uint16_t len = mavlink_msg_to_send_buffer(buffer, &command);
    QByteArray data(reinterpret_cast<char*>(buffer), len);
    if(_device)
        sendRawCommand(data);
    else
        _messageQueue.push(data);
}
