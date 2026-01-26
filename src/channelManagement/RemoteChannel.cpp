#include "RemoteChannel.h"

#include <QVBoxLayout>

#include "componentSystem/Component.h"

#include "vehicle/logs/mavlink/mavlinkvehiclelogs.h"
#include "vehicle/telemetry/mavlink/mavlinkvehicletelemetry.h"
#include "vehicle/statuslog/mavlink/mavlinkvehiclestatuslog.h"
#include "vehicle/parameters/Parameters.h"

ChannelThreadWorker::ChannelThreadWorker(RemoteChannel* channel, QObject* parent) 
    : QThread(parent)
    , _logs(new MavlinkVehicleLogs())
    , _telemetry(new MavlinkVehicleTelemetry())
    , _statusLog(new MavlinkVehicleStatusLog())
    , _parameters(new Parameters())
    , _channel()
{

}

ChannelThreadWorker::~ChannelThreadWorker() {
    if (_logs) {
        _logs->deleteLater();
    }
    if (_telemetry) {
        _telemetry->deleteLater();
    }
    if (_statusLog) {
        _statusLog->deleteLater();
    }
    if (_parameters) {
        _parameters->deleteLater();
    }
}

VehicleLogs* ChannelThreadWorker::getLogs() {
    return _logs;
}

VehicleTelemetry* ChannelThreadWorker::getTelemetry() {
    return _telemetry;
}

VehicleStatusLog* ChannelThreadWorker::getStatusLog() {
    return _statusLog;
}

Parameters* ChannelThreadWorker::getParameters() {
    return _parameters;
}

void ChannelThreadWorker::onEnable() {

}

void ChannelThreadWorker::onDisable() {
    
}

void ChannelThreadWorker::onPacketizerUpdated(BasePacketizer* packetizer) {
    
}

RemoteChannel::RemoteChannel(QWidget* parent) 
    : QWidget(parent)
    , _worker(new ChannelThreadWorker(this))
{
    setLayout(new QVBoxLayout());

    connect(this, &RemoteChannel::packetizerUpdated, _worker, &ChannelThreadWorker::onPacketizerUpdated);
    connect(this, &QObject::destroyed, _worker, &QThread::quit);
    connect(this, &QObject::destroyed, _worker, &QThread::deleteLater);
    _worker->start();
}

RemoteChannel::~RemoteChannel() {

}

ChannelThreadWorker* RemoteChannel::getWorker() {
    return _worker;
}

void RemoteChannel::onAddComponent(Component* component) {
    Component* comp = findChild<Component*>(component->objectName());
    if(!comp) {
        component->setParent(this);
        layout()->addWidget(component);
    } else {
        qWarning() << "Component " << component->objectName() << " already exists";
    }
}

void RemoteChannel::setPacketizer(BasePacketizer* packetizer) {
    emit packetizerUpdated(packetizer);
}