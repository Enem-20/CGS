#ifndef C_MAVLINK_PROTOCOL_FABRIC_H
#define C_MAVLINK_PROTOCOL_FABRIC_H

#include "../ProtocolFabric.h"

class MavlinkProtocolFabric : public ProtocolFabric {
    Q_OBJECT
public:
    explicit MavlinkProtocolFabric(QObject* parent = nullptr);
public slots:
    Vehicle* createVehicle(RemoteChannelID id, QList<uint64_t> layers) override;
};

#endif // C_MAVLINK_PROTOCOL_FABRIC_H