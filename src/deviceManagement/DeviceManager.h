#ifndef C_DEVICE_MANAGER_H
#define C_DEVICE_MANAGER_H

#include <QObject>

#include <QHash>
#include <QSerialPortInfo>


class BaseDevice;
class UDPDevice;
class SerialDevice;

class DeviceManager : public QObject {
    Q_OBJECT

    static DeviceManager* _instance;

    DeviceManager();
    ~DeviceManager();

public:
    static DeviceManager* getInstance();

    void createDefaultDevice();
    BaseDevice* getDevice(uint64_t id);
    BaseDevice* getDefaultDevice();
    UDPDevice* createUDPDevice(QString name, quint16 port, const QString& address);
    SerialDevice* createSerialDevice(QString name, const QSerialPortInfo& portInfo);
    void destroyDevice(uint64_t id);

signals:
    void deviceCreated(uint64_t id);
    void deviceDestroyed(uint64_t id);

private:
    QHash<uint64_t, BaseDevice*> _devices = {};
    BaseDevice* _defaultDevice = nullptr;
    uint64_t _deviceCounter = 0;
};

#endif // C_DEVICE_MANAGER_H
