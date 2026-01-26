#ifndef C_PROTOCOL_FABRIC_H
#define C_PROTOCOL_FABRIC_H

#include <QObject>
#include <QHash>

#include "channelManagement/RemoteChannelID.h"

class Vehicle;

class ProtocolFabric : public QObject {
    Q_OBJECT
public:
    explicit ProtocolFabric(QObject* parent = nullptr);
public slots:
    virtual Vehicle* createVehicle(RemoteChannelID,QList<uint64_t>) = 0;
};

#endif // C_PROTOCOL_FABRIC_H