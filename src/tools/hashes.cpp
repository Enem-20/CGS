#include "hashes.h"

size_t qHash(const VehicleId& id, size_t seed) noexcept {
    return qHashMulti(seed, id.protocolId, id.deviceType, id.deviceId, id.uniqueVehicleId);
}

bool operator==(const VehicleId &lhs, const VehicleId &rhs) noexcept
{
    return
        lhs.protocolId      == rhs.protocolId   &&
        lhs.deviceId        == rhs.deviceId     &&
        lhs.deviceType      == rhs.deviceType   &&
        lhs.uniqueVehicleId == rhs.uniqueVehicleId;
}