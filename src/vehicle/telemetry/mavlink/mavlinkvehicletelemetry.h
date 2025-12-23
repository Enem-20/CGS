#ifndef MAVLINKVEHICLETELEMETRY_H
#define MAVLINKVEHICLETELEMETRY_H

#include "../vehicletelemetry.h"

#include "protocols/mavlink/MavlinkFwd.h"

class MavlinkVehicleTelemetry : public VehicleTelemetry {
    Q_OBJECT

public:
    explicit MavlinkVehicleTelemetry(QObject *parent = nullptr);

public slots:
    void onMessage(Message msg) override;
    void onHeartbeatReceived(Message msg);
    void onAttitudeReceived(Message msg);
    void onLocalPositionNEDReceived(Message msg);
    void onGlobalPositionINTReceived(Message msg);

private:
    void requestTelemetry() override;
    void requestStopTelemetry() override;

    void handleMavlink(const mavlink_heartbeat_t& msg);
    void handleMavlink(const mavlink_attitude_t& msg);
    void handleMavlink(const mavlink_local_position_ned_t& msg);
    void handleMavlink(const mavlink_global_position_int_t& msg);
};

#endif // MAVLINKVEHICLETELEMETRY_H
