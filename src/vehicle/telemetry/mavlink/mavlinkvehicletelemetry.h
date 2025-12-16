#ifndef MAVLINKVEHICLETELEMETRY_H
#define MAVLINKVEHICLETELEMETRY_H

#include "../vehicletelemetry.h"

#include "mavlink/mavlinktypes.h"

class MavlinkVehicleTelemetry : public VehicleTelemetry {
    Q_OBJECT

public:
    explicit MavlinkVehicleTelemetry(QObject *parent = nullptr);

public slots:
    void onMessageReceived(Message msg) override;

private:
    void requestTelemetry() override;

    void handleMavlink(const mavlink_attitude_t& msg);
    void handleMavlink(const mavlink_local_position_ned_t& msg);
    void handleMavlink(const mavlink_global_position_int_t& msg);
};

#endif // MAVLINKVEHICLETELEMETRY_H
