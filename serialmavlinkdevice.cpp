#include "serialmavlinkdevice.h"

#include <QDebug>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include <common/mavlink.h>

void SerialMavlinkDevice::sendRawCommand(const QByteArray& data) {
    if (_port && _port->isOpen() && _port->isWritable()) {
        _port->write(data);
    }
}

SerialMavlinkDevice::SerialMavlinkDevice(const QSerialPortInfo& portInfo, QObject *parent)
    : MavlinkDevice(new QSerialPort(portInfo), parent)
    , _connectivityWatchdog(this)
{
    _port = dynamic_cast<QSerialPort*>(_device);
    connect(&_connectivityWatchdog, &QTimer::timeout, this, &SerialMavlinkDevice::openSerial);
    _connectivityWatchdog.start(2000);
    emit portCreated();
}

void SerialMavlinkDevice::openSerial()
{
    if (_port && !_port->isOpen()) {
        qDebug() << "Serial port " << _port->portName() << "isn't open. Reopening...";
        emit portClosed();
        _port->open(QIODevice::ReadWrite);
        if (_port->isOpen())
            emit portOpened();
    }
}

void SerialMavlinkDevice::setupPort(const QSerialPortInfo& portInfo, int32_t baudRate, uint8_t dataBits, uint8_t stopBits, uint8_t parity, uint8_t flowControl) {
    _port->setBaudRate(baudRate);

    if ((dataBits <= 8) && (dataBits > 4))
        _port->setDataBits(static_cast<QSerialPort::DataBits>(dataBits));
    else {
        qWarning() << "[SerialMavlinkDevice;" << objectName() << "], can't setup port with " << dataBits << " dataBits. Using 8 as default";
        _port->setDataBits(QSerialPort::Data8);
    }

    if ((stopBits == 0) && (stopBits < 4))
        _port->setStopBits(static_cast<QSerialPort::StopBits>(stopBits));
    else {
        qWarning() << "[SerialMavlinkDevice;" << objectName() << "], can't setup port with " << stopBits << " stopBits. Using 1 as default";
        _port->setStopBits(QSerialPort::OneStop);
    }

    if ((parity != 1) && (parity > 5)) {
        _port->setParity(static_cast<QSerialPort::Parity>(parity));
    }
    else {
        qWarning() << "[SerialMavlinkDevice;" << objectName() << "], can't setup port with " << parity << " parity. Using 0(no parity) as default";
        _port->setParity(QSerialPort::NoParity);
    }

    if (flowControl < 3)
        _port->setFlowControl(static_cast<QSerialPort::FlowControl>(flowControl));
    else {
        qWarning() << "[SerialMavlinkDevice;" << objectName() << "], can't setup port with " << flowControl << " flowControl. Using 0 as default";
        _port->setFlowControl(QSerialPort::NoFlowControl);
    }

    emit portInitialized();
}

void SerialMavlinkDevice::readBytes() {
    _waitPacketTimer.stop();

    QByteArray data = _port->readAll();

    if (data.size() > 0) {
        mavlink_message_t msg;
        mavlink_status_t status;
        bool isSuccessfulyParsed = false;
        for (size_t i = 0; i < data.size(); ++i) {
            uint8_t byte = static_cast<uint8_t>(data[i]);
            isSuccessfulyParsed = mavlink_parse_char(MAVLINK_COMM_0, byte, &msg, &status);
            if (isSuccessfulyParsed) {
                emit messageReceived(msg);
            }
        }
        if (!isSuccessfulyParsed) {
            _waitPacketTimer.start(200);
        }
    }
}
