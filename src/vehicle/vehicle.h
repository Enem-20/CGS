#ifndef VEHICLE_H
#define VEHICLE_H

#include <QObject>

class BaseDevice;
class BasePacketizer;
class VehicleLogs;
class VehicleTelemetry;
class VehicleStatusLog;
class Parameters;

class Vehicle : public QObject {
    Q_OBJECT

public:
    explicit Vehicle(QObject *parent = nullptr);
    virtual ~Vehicle();

    virtual VehicleLogs* getLogs();
    virtual VehicleTelemetry* getTelemetry();
    virtual VehicleStatusLog* getStatusLog();
    virtual Parameters* getParameters();

protected:
    BaseDevice* _device;
    BasePacketizer* _packetizer;
    VehicleLogs* _logs;
    VehicleTelemetry* _telemetry;
    VehicleStatusLog* _statusLog;
    Parameters* _parameters;

public slots:
    void setDevice(BaseDevice* device);
    void setPacketizer(BasePacketizer* packetizer);
};

#endif // VEHICLE_H
