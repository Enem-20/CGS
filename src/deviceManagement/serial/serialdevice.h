#ifndef SERIALDEVICE_H
#define SERIALDEVICE_H

#include <QSerialPortInfo>

#include "../basedevice.h"


class QSerialPort;

class SerialDevice : public BaseDevice
{
    Q_OBJECT
private:
    QTimer _connectivityWatchdog;
    QTimer _keepAliveWatchdog;
    PortState _state;
    QSerialPortInfo _portInfo;
    QSerialPort* _port;
protected:
    void sendRawCommand(const QByteArray& data) override;

public:
    explicit SerialDevice(QString name, const QSerialPortInfo& portInfo, QObject *parent = nullptr);

public slots:
    void onOpenSerial();
    void onDisconnected();
    void onSetupPort(const QSerialPortInfo& portInfo, int32_t baudRate, uint8_t dataBits, uint8_t stopBits, uint8_t parity, uint8_t flowControl);


protected slots:
    virtual void onReadBytes() override;
    void onMessageTransmitRequest(Message msg) override;
};

#endif // SERIALDEVICE_H
