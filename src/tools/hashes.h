#pragma once

#ifndef N_HASHES_H
#define N_HASHES_H

#include <QHash>

#include "vehicleManagement/VehicleId.h"

size_t qHash(const VehicleId& id, size_t seed = 0) noexcept;

bool operator==(const VehicleId &lhs, const VehicleId &rhs) noexcept;

namespace hashes {
    consteval uint32_t fnv1a32(const char* str, uint32_t hash = 0x811C9DC5) {
        return *str ? fnv1a32(str + 1, (hash ^ static_cast<uint8_t>(*str)) * 0x01000193) : hash;
    }

    consteval uint64_t fnv1a64(const char* str, uint64_t hash = 0xCBF29CE484222325) {
        return *str ? fnv1a64(str + 1, (hash ^ static_cast<uint8_t>(*str)) * 0x00000100000001B3) : hash;
    }

    consteval uint32_t compileTimeHash(const char* str) {
        uint32_t hash = 5381;
        while(*str) {
            hash = ((hash << 5) + hash) + static_cast<uint8_t>(*str);
            ++str;
        }
        return hash;
    }
}

#endif // N_HASHES_H