#include "mavlinkvehicle.h"

#include "common/mavlink.h"

#include "vehicle/logs/mavlink/mavlinkvehiclelogs.h"
#include "vehicle/telemetry/mavlink/mavlinkvehicletelemetry.h"
#include "vehicle/statuslog/mavlink/mavlinkvehiclestatuslog.h"
#include "vehicle/parameters/Parameters.h"
#include "protocols/mavlink/mavlinkpacketizer.h"

MavlinkVehicle::MavlinkVehicle(QWidget *parent)
    : Vehicle(parent)
{
    //_logs = new MavlinkVehicleLogs();
    //_telemetry = new MavlinkVehicleTelemetry();
    //_statusLog = new MavlinkVehicleStatusLog();
    //_parameters = new Parameters();
}

void MavlinkVehicle::connectPacketizer(MavlinkPacketizer* packetizer) {
    // MavlinkVehicleLogs* mavlinkLogs = qobject_cast<MavlinkVehicleLogs*>(_logs);
    // MavlinkVehicleTelemetry* mavlinkTelemetry = qobject_cast<MavlinkVehicleTelemetry*>(_logs);
    // MavlinkVehicleStatusLog* mavlinkStatusLog = qobject_cast<MavlinkVehicleStatusLog*>(_logs);
    // Parameters* mavlinkParameters = qobject_cast<Parameters*>(_logs);

    // // connect self
    // connect(packetizer, &MavlinkPacketizer::heartbeatReceived, this, &MavlinkVehicle::onHeartbeatReceived);

    // // connect logs
    // connect(mavlinkLogs, &VehicleLogs::sendMessageRequest, packetizer, &MavlinkPacketizer::onSendMessageRequest);
    // connect(packetizer, &MavlinkPacketizer::logEntryReceived, mavlinkLogs, &MavlinkVehicleLogs::onLogEntryReceived);
    // connect(packetizer, &MavlinkPacketizer::logDataReceived, mavlinkLogs, &MavlinkVehicleLogs::onLogDataReceived);

    // // connect telemetry
    // connect(mavlinkTelemetry, &MavlinkVehicleTelemetry::sendMessageRequest, packetizer, &MavlinkPacketizer::onSendMessageRequest);
    // connect(packetizer, &MavlinkPacketizer::heartbeatReceived, mavlinkTelemetry, &MavlinkVehicleTelemetry::onHeartbeatReceived);
    // connect(packetizer, &MavlinkPacketizer::attitudeReceived, mavlinkTelemetry, &MavlinkVehicleTelemetry::onAttitudeReceived);
    // connect(packetizer, &MavlinkPacketizer::localPositionNEDReceived, mavlinkTelemetry, &MavlinkVehicleTelemetry::onLocalPositionNEDReceived);
    // connect(packetizer, &MavlinkPacketizer::globalPositionINTReceived, mavlinkTelemetry, &MavlinkVehicleTelemetry::onGlobalPositionINTReceived);
    
    // // connect status log
    // connect(mavlinkStatusLog, &MavlinkVehicleStatusLog::sendMessageRequest, packetizer, &MavlinkPacketizer::onSendMessageRequest);
    // connect(packetizer, &MavlinkPacketizer::statusTextReceived, mavlinkStatusLog, &MavlinkVehicleStatusLog::onStatusTextReceived);

    // // connect parameters
    // connect(mavlinkParameters, &Parameters::sendMessageRequest, packetizer, &MavlinkPacketizer::onSendMessageRequest);
    // connect(packetizer, &MavlinkPacketizer::paramValueReceived, mavlinkParameters, &Parameters::onMessage);

    // // connect system interactions
    // connect(mavlinkParameters, &Parameters::parametersPullingStarted, mavlinkTelemetry, &MavlinkVehicleTelemetry::onStopTelemetry);
    // connect(mavlinkParameters, &Parameters::parametersPullingCompleted, mavlinkTelemetry, &MavlinkVehicleTelemetry::onStartTelemetry);
    
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
