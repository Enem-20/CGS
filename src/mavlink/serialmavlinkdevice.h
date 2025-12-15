#ifndef SERIALMAVLINKDEVICE_H
#define SERIALMAVLINKDEVICE_H

#include <QTimer>
#include <QtSerialPort/QSerialPortInfo>

#include "mavlinkdevice.h"

class QSerialPort;

class SerialMavlinkDevice : public MavlinkDevice {
    Q_OBJECT

private:
    QTimer _connectivityWatchdog;
    QTimer _keepAliveWatchdog;
    PortState_ _state;
    QSerialPortInfo _portInfo;
    QSerialPort* _port;

protected:
    void sendRawCommand(const QByteArray& data) override;

public:
    explicit SerialMavlinkDevice(QString name, const QSerialPortInfo& portInfo, QObject *parent = nullptr);

public slots:
    void openSerial();
    void onDisconnected();
    void setupPort(const QSerialPortInfo& portInfo, int32_t baudRate, uint8_t dataBits, uint8_t stopBits, uint8_t parity, uint8_t flowControl);


protected slots:
    virtual void readBytes() override;
};

#endif // SERIALMAVLINKDEVICE_H
