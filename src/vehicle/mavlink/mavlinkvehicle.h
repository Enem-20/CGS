#ifndef MAVLINKVEHICLE_H
#define MAVLINKVEHICLE_H

#include "../vehicle.h"

#include <QVector>
#include <QPair>

#include "protocols/message.h"

class MavlinkVehicle : public Vehicle {
    Q_OBJECT

    QVector<QPair<uint32_t, uint32_t>> _detectedComponents;

    uint8_t _sysId;
    uint8_t _compId;
    
public:
    explicit MavlinkVehicle(QObject *parent = nullptr);

private slots:
    void onHeartbeatReceived(Message msg);
};

#endif // MAVLINKVEHICLE_H
