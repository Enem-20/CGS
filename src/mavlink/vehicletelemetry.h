#ifndef VEHICLETELEMETRY_H
#define VEHICLETELEMETRY_H

#include <QObject>

#include <QVector3D>

#include "mavlinksubscriber.h"

class VehicleTelemetry : public MavlinkSubscriber {
    Q_OBJECT

public:
    explicit VehicleTelemetry(QObject *parent = nullptr);

    QVector<uint32_t> getSubscribtionMessageIds() const override;

private:
    QVector3D _orientation;
    QVector3D _speeds;
    int32_t _altitude;

    void handleMavlink(const mavlink_attitude_t& msg);
    void handleMavlink(const mavlink_local_position_ned_t& msg);
    void handleMavlink(const mavlink_global_position_int_t& msg);

public slots:
    void onMessageReceived(const mavlink_message_t& message) override;

signals:
    void orientationUpdated(const QVector3D& orientation);
    void speedsUpdated(const QVector3D& orientation);
    void altitudeUpdated(int32_t altitude);
};

#endif // VEHICLETELEMETRY_H
