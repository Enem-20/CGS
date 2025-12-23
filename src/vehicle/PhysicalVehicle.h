#ifndef C_PHYSICAL_VEHICLE_H
#define C_PHYSICAL_VEHICLE_H

#include <cstdint>

#include <QObject>
#include <QSet>

#include "tools/hashes.h"
#include "vehicleManagement/VehicleId.h"

class Vehicle;
class BaseDevice;

class PhysicalVehicle : public QObject {
    Q_OBJECT

    typedef QHash<uint64_t, Vehicle*> DeviceMap;
    typedef QHash<uint64_t, DeviceMap> DeviceTypeMap;
    typedef QHash<uint64_t, DeviceTypeMap> ProtocolMap;

    uint64_t _sysId;
    ProtocolMap _virtualVehicles;
    QHash<VehicleId, bool> _registeredIds;
    bool _enabled = true;
    
public:
    PhysicalVehicle();

    void registerId(VehicleId id);
    void deactivateId(VehicleId id);
    void unregisterId(VehicleId id);
    bool is(VehicleId id);
    bool hasRegisteredIds();

signals:
    void enabled(PhysicalVehicle* physicalVehicle);
    void disabled(PhysicalVehicle* physicalVehicle);

public slots:
    void onEnable();
    void onDisable(); 
    void onVehicleId(VehicleId id, BaseDevice* device);

private:
    void createVehicle(VehicleId id);
    void destroyVehicle(VehicleId id);
};

#endif // C_PHYSICAL_VEHICLE_H