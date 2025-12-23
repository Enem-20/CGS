#ifndef VEHICLETELEMETRY_H
#define VEHICLETELEMETRY_H

#include "protocols/ProtocolSubscriber.h"

#include <QVector3D>

#include "protocols/message.h"

class VehicleTelemetry : public ProtocolSubscriber {
    Q_OBJECT

public:
    explicit VehicleTelemetry(QObject *parent = nullptr);

protected:
    QVector3D _orientation;
    QVector3D _velocityNED;
    QVector3D _positionNED;
    float _altitude;
    float _relativeAltitude;
    float _heading;
    float _latitude;
    float _longitude;
    uint32_t _mode;
    bool _armed;

signals:
    void orientationUpdated(const QVector3D& orientation, uint64_t timestamp);
    void velocityUpdated(const QVector3D& velocity, uint64_t timestamp);
    void altitudeUpdated(float altitude, uint64_t timestamp);
    void modeUpdated(uint32_t mode);
    void armStateUpdated(bool state);

public slots:
    void onStartTelemetry();
    void onStopTelemetry();

    void onMessage(Message msg) override;
private:
    virtual void requestTelemetry() = 0;
    virtual void requestStopTelemetry() = 0;
};

#endif // VEHICLETELEMETRY_H
