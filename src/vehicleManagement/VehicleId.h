#ifndef S_VEHICLE_ID_H
#define S_VEHICLE_ID_H

#include <cstdint>
#include <QHash>

struct VehicleId {
    uint64_t protocolId;
    uint64_t deviceType;
    uint64_t deviceId;
    uint64_t uniqueVehicleId;
};

#endif // S_VEHICLE_ID_H