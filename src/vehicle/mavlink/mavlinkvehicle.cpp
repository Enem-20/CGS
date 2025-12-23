#include "mavlinkvehicle.h"

#include "common/mavlink.h"

#include "deviceManagement/basedevice.h"
#include "../logs/mavlink/mavlinkvehiclelogs.h"
#include "../telemetry/mavlink/mavlinkvehicletelemetry.h"
#include "../statuslog/mavlink/mavlinkvehiclestatuslog.h"
#include "../parameters/Parameters.h"
#include "protocols/mavlink/mavlinkpacketizer.h"

MavlinkVehicle::MavlinkVehicle(QObject *parent)
    : Vehicle(new MavlinkPacketizer(nullptr), parent)
{
    MavlinkPacketizer* mavlinkPacketizer = qobject_cast<MavlinkPacketizer*>(_packetizer);
    //mavlinkPacketizer->setParent(this);
    MavlinkVehicleLogs* mavlinkLogs = new MavlinkVehicleLogs();
    MavlinkVehicleTelemetry* mavlinkTelemetry = new MavlinkVehicleTelemetry();
    MavlinkVehicleStatusLog* mavlinkStatusLog = new MavlinkVehicleStatusLog();
    Parameters* mavlinkParameters = new Parameters();

    _logs = mavlinkLogs;
    _telemetry = mavlinkTelemetry;
    _statusLog = mavlinkStatusLog;
    _parameters = mavlinkParameters;

    // connect self
    connect(mavlinkPacketizer, &MavlinkPacketizer::heartbeatReceived, this, &MavlinkVehicle::onHeartbeatReceived);

    // connect logs
    connect(mavlinkLogs, &VehicleLogs::sendMessageRequest, mavlinkPacketizer, &MavlinkPacketizer::onSendMessageRequest);
    connect(mavlinkPacketizer, &MavlinkPacketizer::logEntryReceived, mavlinkLogs, &MavlinkVehicleLogs::onLogEntryReceived);
    connect(mavlinkPacketizer, &MavlinkPacketizer::logDataReceived, mavlinkLogs, &MavlinkVehicleLogs::onLogDataReceived);

    // connect telemetry
    connect(mavlinkTelemetry, &MavlinkVehicleTelemetry::sendMessageRequest, mavlinkPacketizer, &MavlinkPacketizer::onSendMessageRequest);
    connect(mavlinkPacketizer, &MavlinkPacketizer::heartbeatReceived, mavlinkTelemetry, &MavlinkVehicleTelemetry::onHeartbeatReceived);
    connect(mavlinkPacketizer, &MavlinkPacketizer::attitudeReceived, mavlinkTelemetry, &MavlinkVehicleTelemetry::onAttitudeReceived);
    connect(mavlinkPacketizer, &MavlinkPacketizer::localPositionNEDReceived, mavlinkTelemetry, &MavlinkVehicleTelemetry::onLocalPositionNEDReceived);
    connect(mavlinkPacketizer, &MavlinkPacketizer::globalPositionINTReceived, mavlinkTelemetry, &MavlinkVehicleTelemetry::onGlobalPositionINTReceived);
    
    // connect status log
    connect(mavlinkStatusLog, &MavlinkVehicleStatusLog::sendMessageRequest, mavlinkPacketizer, &MavlinkPacketizer::onSendMessageRequest);
    connect(mavlinkPacketizer, &MavlinkPacketizer::statusTextReceived, mavlinkStatusLog, &MavlinkVehicleStatusLog::onStatusTextReceived);

    // connect parameters
    connect(mavlinkParameters, &Parameters::sendMessageRequest, mavlinkPacketizer, &MavlinkPacketizer::onSendMessageRequest);
    connect(mavlinkPacketizer, &MavlinkPacketizer::paramValueReceived, mavlinkParameters, &Parameters::onMessage);

    // connect system interactions
    connect(mavlinkParameters, &Parameters::parametersPullingStarted, mavlinkTelemetry, &MavlinkVehicleTelemetry::onStopTelemetry);
    connect(mavlinkParameters, &Parameters::parametersPullingCompleted, mavlinkTelemetry, &MavlinkVehicleTelemetry::onStartTelemetry);
    
    // connect(_packetizer, &BasePacketizer::messageReceived, _logs, &VehicleLogs::onMessage);
    // connect(_packetizer, &BasePacketizer::messageReceived, _telemetry, &VehicleTelemetry::onMessage);
    // connect(_packetizer, &BasePacketizer::messageReceived, _statusLog, &VehicleStatusLog::onMessage);
    // connect(_packetizer, &BasePacketizer::paramValueReceived, _parameters, &Parameters::onMessage);
}

void MavlinkVehicle::onHeartbeatReceived(Message msg) {
    MavlinkMessage* message = msg.read<MavlinkMessage>();
    mavlink_heartbeat_t heartbeat;
    mavlink_msg_heartbeat_decode(message, &heartbeat);

    auto it = std::find_if(_detectedComponents.begin(), _detectedComponents.end(), [this, message](QPair<uint32_t, uint32_t> el) {
        if (message->compid == el.first) {
            return true;
        }
        return false;
    });

    if (it == _detectedComponents.end()) {
        qDebug() << "New component detected. id: " << message->compid << " type: " << heartbeat.type;
        _detectedComponents.append({message->compid, heartbeat.type});
    }
}
