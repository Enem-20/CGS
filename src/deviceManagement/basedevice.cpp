#include "basedevice.h"

#include <QIODevice>

BaseDevice::BaseDevice(const QString& name, const QString& type, QIODevice* device, QObject *parent)
    : QThread(parent)
    , _device(device)
    , _queueSendTimer(parent)
    , _connectionWatchdog(parent)
    , _waitPacketTimer(parent)
    , _name(name)
    , _type(type)
{

}

BaseDevice::~BaseDevice() {
    if(_device)
        _device->deleteLater();
}

QStringView BaseDevice::getName() const {
    return _name;
}

QStringView BaseDevice::getType() const {
    return _type;
}

void BaseDevice::onWaitPacketTimeout() {
    _inputBuffer.clear();
}
