#include "MavlinkProtocol.h"

#include "tools/hashes.h"
#include "MavlinkProtocolFabric.h"

MavlinkProtocol::MavlinkProtocol(QObject* parent)
    : Protocol(GENERATE_HASH(MavlinkProtocol), new MavlinkProtocolFabric(parent), parent)
{}