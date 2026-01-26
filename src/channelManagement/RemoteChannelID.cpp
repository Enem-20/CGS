#include "RemoteChannelID.h"

size_t qHash(const RemoteChannelID& id, size_t seed) noexcept {
    return qHashMulti(seed, id._protocolID, id._portType, id._portID, id._sourceID);
}

bool operator==(const RemoteChannelID &lhs, const RemoteChannelID &rhs) noexcept {
    return
        lhs._protocolID      == rhs._protocolID     &&
        lhs._portID        == rhs._portID           &&
        lhs._portType      == rhs._portType         &&
        lhs._sourceID == rhs._sourceID;
}