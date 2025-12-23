#ifndef VEHICLE_H
#define VEHICLE_H

#include <QObject>
#include <QHash>

#include "vehicleManagement/VehicleId.h"

class BaseDevice;
class BasePacketizer;
class VehicleLogs;
class VehicleTelemetry;
class VehicleStatusLog;
class Parameters;

class Vehicle : public QObject {
    Q_OBJECT
    QHash<QString, BaseDevice*> _devices;
public:
    explicit Vehicle(BasePacketizer* packetizer, QObject *parent = nullptr);
    virtual ~Vehicle();

    virtual BasePacketizer* getPacketizer();
    virtual VehicleLogs* getLogs();
    virtual VehicleTelemetry* getTelemetry();
    virtual VehicleStatusLog* getStatusLog();
    virtual Parameters* getParameters();

protected:
    BaseDevice* _active = nullptr;
    BasePacketizer* _packetizer = nullptr;
    VehicleLogs* _logs = nullptr;
    VehicleTelemetry* _telemetry = nullptr;
    VehicleStatusLog* _statusLog = nullptr;
    Parameters* _parameters = nullptr;
    
public slots:
    void setActiveDevice(BaseDevice* device);
    //void addDevice(VehicleId id, BaseDevice* device);
};

#endif // VEHICLE_H
