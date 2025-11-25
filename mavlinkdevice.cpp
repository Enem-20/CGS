#include "mavlinkdevice.h"

#include <QIODevice>
#include <common/mavlink.h>

MavlinkDevice::MavlinkDevice(QIODevice* device, QObject *parent)
    : QObject(parent)
    , _queueSendTimer(parent)
    , _connectionWatchdog(parent)
    , _waitPacketTimer(parent)
{
    _device = device;
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
    delete _device;
}

void MavlinkDevice::readBytes() {
    _inputBuffer += _device->readAll();
    const uint8_t* bytedInputBuffer = reinterpret_cast<const uint8_t*>(_inputBuffer.constData());

    _waitPacketTimer.start(200);

    if(bytedInputBuffer[0] != MAVLINK_STX) return;
    uint8_t expectedLen = bytedInputBuffer[1];
    if(_inputBuffer < expectedLen + MAVLINK_NUM_NON_PAYLOAD_BYTES) return;

    mavlink_status_t status;
    mavlink_message_t result;
    for(size_t i = 0; i < expectedLen; ++i) {
        mavlink_parse_char(MAVLINK_COMM_0, bytedInputBuffer[i], &result, &status);
    }

    emit messageReceived(result);
}

void MavlinkDevice::waitPacketTimeout() {
    _inputBuffer.clear();
}

void MavlinkDevice::sendCommand(const mavlink_message_t& command) {
    uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
    uint16_t len = mavlink_msg_to_send_buffer(buffer, &command);
    QByteArray data(reinterpret_cast<char*>(buffer), len);
    if(_device)
        _device->write(data);
    else
        _messageQueue.push(data);
}
