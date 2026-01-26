#include "DeviceManager.h"

#include <QDebug>

#include "basedevice.h"

#include "serial/serialdevice.h"
#include "udp/udpdevice.h"

DeviceManager* DeviceManager::_instance = nullptr;

DeviceManager::DeviceManager() 
    : QObject(nullptr) 
{

}

DeviceManager::~DeviceManager() {
    for (BaseDevice* device : _devices.values()) {
        device->deleteLater();
    }
}

DeviceManager* DeviceManager::getInstance() {
    if (_instance) [[likely]] {
        return _instance;
    }
    _instance = new DeviceManager();
    return _instance;
}

void DeviceManager::createDefaultDevice() {
    if (!_defaultDevice) {
        _defaultDevice = createUDPDevice("DefaultDevice", 14550, "127.0.0.1");
    }
}

BaseDevice* DeviceManager::getDevice(uint64_t id) {
    auto it = _devices.find(id);
    if (it != _devices.end()) {
        return it.value();
    }
    return nullptr;
}

BaseDevice* DeviceManager::getDefaultDevice() {
    return _defaultDevice;
}

UDPDevice* DeviceManager::createUDPDevice(QString name, quint16 port, const QString& address) {
    UDPDevice* device = new UDPDevice(name, port, address);
    uint64_t deviceID = reinterpret_cast<uint64_t>(device);
    uint64_t id = _deviceCounter++;
    _devices.insert(id, device);
    emit deviceCreated(id);
    device->start();
    return device;
}

SerialDevice* DeviceManager::createSerialDevice(QString name, const QSerialPortInfo& portInfo) {
    qDebug() << "Hello from createSerialDevice!";
    SerialDevice* device = new SerialDevice(name, portInfo);
    uint64_t id = _deviceCounter++;
    _devices.insert(id, device);
    emit deviceCreated(id);
    device->start();
    return device;
}
