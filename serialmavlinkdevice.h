#ifndef SERIALMAVLINKDEVICE_H
#define SERIALMAVLINKDEVICE_H

#include <QTimer>

#include "mavlinkdevice.h"

class QSerialPort;
class QSerialPortInfo;

enum class PortState : uint8_t {
    Uninitialized,
    Initialized,
    Opened
};

class SerialMavlinkDevice : public MavlinkDevice {
    Q_OBJECT

private:
    QTimer _connectivityWatchdog;
    PortState _state;
    QSerialPort* _port;

protected:
    void sendRawCommand(const QByteArray& data) override;

public:
    explicit SerialMavlinkDevice(const QSerialPortInfo& portInfo, QObject *parent = nullptr);

signals:
    void portStateChanged(PortState state);
    void portCreated();
    void portInitialized();
    void portOpened();
    void portClosed();

public slots:
    void openSerial();
    void setupPort(const QSerialPortInfo& portInfo, int32_t baudRate, uint8_t dataBits, uint8_t stopBits, uint8_t parity, uint8_t flowControl);

protected slots:
    virtual void readBytes() override;
};

#endif // SERIALMAVLINKDEVICE_H
