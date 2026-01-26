#ifndef MAVLINKVEHICLE_H
#define MAVLINKVEHICLE_H

#include "../Vehicle.h"

#include <QVector>
#include <QPair>

#include "protocols/message.h"

class MavlinkPacketizer;

class MavlinkVehicle : public Vehicle {
    Q_OBJECT

    QVector<QPair<uint32_t, uint32_t>>  _detectedComponents;
public:
    explicit MavlinkVehicle(QWidget *parent = nullptr);

    void connectPacketizer(MavlinkPacketizer* packetizer);

public slots:
    void onHeartbeatReceived(Message msg);
};

#endif // MAVLINKVEHICLE_H
