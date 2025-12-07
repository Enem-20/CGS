#ifndef MAVLINKDEVICE_H
#define MAVLINKDEVICE_H

#include <queue>
#include <QObject>
#include <QTimer>

struct __mavlink_message;
typedef __mavlink_message mavlink_message_t;

struct __mavlink_status;
typedef __mavlink_status mavlink_status_t;

class QIODevice;

class MavlinkDevice : public QObject
{
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
protected:
    virtual void sendRawCommand(const QByteArray& data) = 0;
public:
    explicit MavlinkDevice(QIODevice* device, QObject *parent = nullptr);
    virtual ~MavlinkDevice();

signals:
    void messageReceived(mavlink_message_t message);
protected slots:
    virtual void readBytes() = 0;
    virtual void waitPacketTimeout();
public slots:
    virtual void sendCommand(const mavlink_message_t& command);
};

#endif // MAVLINKDEVICE_H
