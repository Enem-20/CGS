#include "ChannelManager.h"

#include <QSqlQuery>

#include "./RemoteChannel.h"
#include "deviceManagement/DeviceManager.h"
#include "protocols/mavlink/MavlinkProtocol.h"

ChannelManager* ChannelManager::_instance = nullptr;

ChannelManager::ChannelManager(QObject* parent) {
    Protocol* mavlinkProtocol = MavlinkProtocol::getInstance<MavlinkProtocol>(this);
    _protocols.emplace(mavlinkProtocol->getTypeHash(), mavlinkProtocol);

    _db = QSqlDatabase::addDatabase("QSQLITE", "MEMORY_CONNECTION");
    _db.setDatabaseName(":memory:");
    if (!_db.open()) {
        qDebug() << "Failed to open in-memory database";
        return;
    }

    QSqlQuery q(_db);
    q.exec("CREATE TABLE channels (channelID INTEGER portID INTEGER portType INTEGER protocolID INTEGER);");
}

ChannelManager* ChannelManager::getInstance() {
    if (_instance) [[likely]]
        return _instance;
    _instance = new ChannelManager();
    _instance->start(QThread::LowPriority);
    return _instance;
}

RemoteChannel* ChannelManager::getChannel(RemoteChannelID id) {
    //if (_vehicles.contains(id)) {
    //    return _vehicles[id];
    //}
    return nullptr;
}

void ChannelManager::createChannel(RemoteChannelID channelId) {
    if(_channels.contains(channelId)) {
        qDebug() << "Channel with specified ID already exists\n";
    }

    

    QSqlQuery q(_db);
    QString query = QString("INSERT INTO channels (channelID portID portType protocolID) VALUES (%1 %2 %3 %4);")
    .arg(channelId._channelID).arg(channelId._portID).arg(channelId._portType).arg(channelId._protocolID);
    q.exec(query);

    // auto protocolIt = _protocols.find(id.protocolId);
    // if(protocolIt != _protocols.end()) {
    //     return protocolIt.value()->getFactory()->createVehicle(id, _layers);
    // } else {
    //     qWarning() << "Failed to create vehicle: Unknown protocol id";
    //     return nullptr;
    // }
}

void ChannelManager::onChannelDetected(RemoteChannelID channelId) {
    //auto vehicleIt = _vehicles.find(channelId);
    //if (vehicleIt != _vehicles.end()) {
    //    BaseDevice* device = DeviceManager::getInstance()->getDevice(portID);
    //    if (device) {
    //        Vehicle* vehicle = vehicleIt.value();
    //        // vehicle->onDeviceConnect(id, device);
    //    }
    //    return;
    //} 
    //createVehicle(id);
}

void ChannelManager::onChannelLost(RemoteChannelID channelId) {

}
