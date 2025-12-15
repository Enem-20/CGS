#ifndef MAVLINKVEHICLE_H
#define MAVLINKVEHICLE_H

#include "../vehicle.h"

class MavlinkVehicle : public Vehicle
{
    Q_OBJECT
    uint8_t _sysId;
    uint8_t _compId;
public:
    explicit MavlinkVehicle(QObject *parent = nullptr);

signals:
public slots:
    void getLog(size_t index) override;
    void getLogs() override;
};

#endif // MAVLINKVEHICLE_H
