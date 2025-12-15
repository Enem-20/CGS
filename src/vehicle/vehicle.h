#ifndef VEHICLE_H
#define VEHICLE_H

#include <QObject>

class BaseDevice;

class Vehicle : public QObject
{
    Q_OBJECT
    BaseDevice* _device;
public:
    explicit Vehicle(QObject *parent = nullptr);

signals:
public slots:
    virtual void getLog(size_t index) = 0;
    virtual void getLogs() = 0;
    void setDevice(BaseDevice* device);
};

#endif // VEHICLE_H
