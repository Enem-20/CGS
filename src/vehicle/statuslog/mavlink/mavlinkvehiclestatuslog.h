#ifndef MAVLINKVEHICLESTATUSLOG_H
#define MAVLINKVEHICLESTATUSLOG_H

#include "../vehiclestatuslog.h"

#include "mavlink/mavlinktypes.h"

class MavlinkVehicleStatusLog : public VehicleStatusLog {
    Q_OBJECT

public:
    explicit MavlinkVehicleStatusLog(QObject *parent = nullptr) noexcept;

public slots:
    void onMessage(Message msg) override;

private:
    void handleMavlink(mavlink_statustext_t message);
};

#endif // MAVLINKVEHICLESTATUSLOG_H
