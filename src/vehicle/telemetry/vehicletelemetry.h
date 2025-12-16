#ifndef VEHICLETELEMETRY_H
#define VEHICLETELEMETRY_H

#include <QObject>

#include <QVector3D>

#include "protocols/message.h"

class VehicleTelemetry : public QObject {
    Q_OBJECT

public:
    explicit VehicleTelemetry(QObject *parent = nullptr);

protected:
    QVector3D _orientation;
    QVector3D _speeds;
    int32_t _altitude;

signals:
    void sendMessage(Message msg);
    void orientationUpdated(const QVector3D& orientation);
    void speedsUpdated(const QVector3D& orientation);
    void altitudeUpdated(int32_t altitude);

public slots:
    virtual void onMessageReceived(Message msg) = 0;

private:
    virtual void requestTelemetry() = 0;
};

#endif // VEHICLETELEMETRY_H
