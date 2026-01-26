#include "basedevice.h"

#include <QIODevice>

#include "protocols/ProtocolMonitor.h"

BaseDevice::BaseDevice(const QString& name, QString type, uint64_t typeHash, QIODevice* device, QObject *parent)
    : QThread(parent)
    , _device(device)
    , _queueSendTimer(parent)
    , _connectionWatchdog(parent)
    , _waitPacketTimer(parent)
    , _name(name)
    , _type(type)
    , _typeHash(typeHash)
{
    _protocolMonitor = new ProtocolMonitor(this);
}

BaseDevice::~BaseDevice() {
    if (_device) {
        _device->deleteLater();
    }
    if (_protocolMonitor) {
        _protocolMonitor->deleteLater();
    }
}

QStringView BaseDevice::getName() const {
    return _name;
}

QStringView BaseDevice::getType() const {
    return _type;
}

uint64_t BaseDevice::getTypeHash() const {
    return _typeHash;
}

ProtocolMonitor* BaseDevice::getProtocolMonitor() {
    return _protocolMonitor;
}

void BaseDevice::onWaitPacketTimeout() {
    _inputBuffer.clear();
}
