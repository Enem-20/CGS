#ifndef VEHICLE_H
#define VEHICLE_H

#include <QObject>

class BaseDevice;
class VehicleLogs;

class Vehicle : public QObject {
    Q_OBJECT

protected:
    BaseDevice* _device = nullptr;
    VehicleLogs* _logs = nullptr;

public:
    explicit Vehicle(QObject *parent = nullptr);
    virtual ~Vehicle();

    VehicleLogs* getLogs();

public slots:
    void setDevice(BaseDevice* device);
};

#endif // VEHICLE_H
