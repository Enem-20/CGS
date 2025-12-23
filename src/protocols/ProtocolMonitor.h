#ifndef C_PROTOCOL_MONITOR_H
#define C_PROTOCOL_MONITOR_H

#include <QObject>
#include <QTimer>

#include "message.h"
#include "tools/hashes.h"
#include "vehicleManagement/VehicleId.h"

class MavlinkPacketizer;
class BasePacketizer;
class BaseDevice;

class ProtocolMonitor : public QObject {
    Q_OBJECT

    MavlinkPacketizer*  _mavlinkPacketizer;
    BaseDevice*         _device;

    QHash<VehicleId, QTimer*> _detectedVehiclesTimers;

public:
    ProtocolMonitor(BaseDevice* device);

signals:
    void vehicleDetected(VehicleId id);
    void vehicleLost(VehicleId id);
    
    bool checkContainsBlockable(VehicleId id);
    void createVehicle(VehicleId id);
    void packetizerUpdate(BasePacketizer* packetizer);

private slots:
    void onHeartbeatMessage(Message msg);
    void onVehicleLost(VehicleId id);
};

#endif // C_PROTOCOL_MONITOR_H