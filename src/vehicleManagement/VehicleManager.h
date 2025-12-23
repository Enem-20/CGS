#ifndef C_VEHICLE_MANAGER_HPP
#define C_VEHICLE_MANAGER_HPP

#include <QThread>

#include "tools/hashes.h"
#include "protocols/message.h"
#include "VehicleId.h"

class PhysicalVehicle;

class VehicleManager : public QThread {
    QHash<uint64_t, PhysicalVehicle*> _vehicles;
    static VehicleManager* _instance;

public:
    VehicleManager();

    static VehicleManager* getInstance();

public slots:
    void onVehicleDetected(VehicleId id);
    void onVehicleLost(VehicleId id);
    void onVehicleConnect(VehicleId id);
    void onVehicleDisconnect(VehicleId id);

    bool contains(VehicleId id) const;
    void create(VehicleId id);
};

#endif // C_VEHICLE_MANAGER_HPP