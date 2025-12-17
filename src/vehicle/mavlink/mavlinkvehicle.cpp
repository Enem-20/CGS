#include "mavlinkvehicle.h"

#include "../logs/mavlink/mavlinkvehiclelogs.h"
#include "../telemetry/mavlink/mavlinkvehicletelemetry.h"
#include "../statuslog/mavlink/mavlinkvehiclestatuslog.h"
#include "../parameters/Parameters.h"
#include "deviceManagement/basedevice.h"

MavlinkVehicle::MavlinkVehicle(QObject *parent)
    : Vehicle{parent}
{
    _logs = new MavlinkVehicleLogs();
    _telemetry = new MavlinkVehicleTelemetry();
    _statusLog = new MavlinkVehicleStatusLog();
    _parameters = new Parameters();
}
