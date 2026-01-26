#ifndef C_REMOTE_CHANNEL_H
#define C_REMOTE_CHANNEL_H

#include <QWidget>
#include <QHash>
#include <QString>
#include <QThread>

class Component;
class VehicleLogs;
class VehicleTelemetry;
class VehicleStatusLog;
class Parameters;
class RemoteChannel;
class BasePacketizer;

class ChannelThreadWorker : public QThread {
    Q_OBJECT
    uint64_t _id;

    VehicleLogs* _logs;
    VehicleTelemetry* _telemetry;
    VehicleStatusLog* _statusLog;
    Parameters* _parameters;
    RemoteChannel* _channel;
public:
    ChannelThreadWorker(RemoteChannel* channel, QObject* parent = nullptr);
    ~ChannelThreadWorker();

    VehicleLogs* getLogs();
    VehicleTelemetry* getTelemetry();
    VehicleStatusLog* getStatusLog();
    Parameters* getParameters();
protected:
    bool _enabled;
    
signals:
    void enabled();
    void disabled();

public slots:
    void onEnable();
    void onDisable();

    void onPacketizerUpdated(BasePacketizer* packetizer);
};

class RemoteChannel : public QWidget {
    Q_OBJECT
    static uint64_t _counter;
private:
    ChannelThreadWorker* _worker;
public:
    explicit RemoteChannel(QWidget* parent = nullptr);
    ~RemoteChannel();

    ChannelThreadWorker* getWorker();
signals:
    void packetizerUpdated(BasePacketizer* packetizer);
public slots:
    void onAddComponent(Component* component);
    void setPacketizer(BasePacketizer* packetizer);
};

#endif // C_REMOTE_CHANNEL_H