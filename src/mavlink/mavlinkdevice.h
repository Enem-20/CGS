#ifndef MAVLINKDEVICE_H
#define MAVLINKDEVICE_H

#include <queue>
#include <QThread>
#include <QTimer>

#include "mavlinktypes.h"

class QIODevice;

enum class PortState : uint8_t {
    Uninitialized,
    Initialized,
    Opened
};

struct MavlinkDeviceInfo {
    QString name;
    QString type;
};

static const QString DEFAULT_DEVICE_NAME = "DefaultDevice";

class MavlinkDevice : public QThread {
    Q_OBJECT

private:
    QTimer _queueSendTimer;
    QTimer _connectionWatchdog;
    bool _packageisValid = true;

protected:
    std::queue<QByteArray> _messageQueue;
    QByteArray _inputBuffer;
    QTimer _waitPacketTimer;
    QIODevice* _device;
    mavlink_status_t* _mavlinkStatus;
    QString _name;
    QString _type;
    uint8_t _sysid = 255;
    uint8_t _compid = 255;

protected:
    virtual void sendRawCommand(const QByteArray& data) = 0;

public:
    explicit MavlinkDevice(const QString& name, const QString& type, QIODevice* device, QObject *parent = nullptr);
    virtual ~MavlinkDevice();

    uint8_t getSysId() const;
    uint8_t getCompId() const;
    QStringView getName() const;
    QStringView getType() const;

signals:
    void messageReceived(mavlink_message_t message);
    void portStateChanged(PortState state);
    void portCreated();
    void portInitialized();
    void portOpened();
    void portClosed();

protected slots:
    virtual void readBytes() = 0;
    virtual void waitPacketTimeout();

public slots:
    virtual void sendCommand(const mavlink_message_t& command);
};

#endif // MAVLINKDEVICE_H
