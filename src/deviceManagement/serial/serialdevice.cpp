#include "serialdevice.h"

#include <QDebug>
#include <QtSerialPort/QSerialPort>

#include <common/mavlink.h>

#include "protocols/basepacketizer.h"

void SerialDevice::sendRawCommand(const QByteArray& data) {
    if (_port && _port->isOpen() && _port->isWritable()) {
        _port->write(data);
    }
}

SerialDevice::SerialDevice(QString name, const QSerialPortInfo& portInfo, QObject *parent)
    : BaseDevice(name, "Serial", new QSerialPort(portInfo, parent), parent)
    , _connectivityWatchdog(this)
    , _portInfo(portInfo)
{
    _port = dynamic_cast<QSerialPort*>(_device);
    connect(&_connectivityWatchdog, &QTimer::timeout, this, &SerialDevice::onOpenSerial);
    connect(&_keepAliveWatchdog, &QTimer::timeout, this, &SerialDevice::onDisconnected);
    connect(_port, &QSerialPort::errorOccurred, this, [this](QSerialPort::SerialPortError err) {
        qDebug() << "serial port error: " << err;
    });
    _connectivityWatchdog.start(5000);
    emit portCreated();
    emit portStateChanged(PortState::Uninitialized);
}

void SerialDevice::onOpenSerial() {
    _connectivityWatchdog.start(5000);
    if (_port && !_port->isOpen()) {
        qDebug() << "Serial port " << _port->portName() << "isn't open. Reopening...";
        qDebug() << "Serial port opening with baudrate: " << _port->baudRate();
        emit portClosed();
        emit portStateChanged(PortState::Uninitialized);
        _port->open(QIODevice::ReadWrite);
        if (_port->isOpen()) {
            _connectivityWatchdog.stop();
            _keepAliveWatchdog.start(8000);
            emit portOpened();
            emit portStateChanged(PortState::Opened);
        }
    }
}

void SerialDevice::onDisconnected() {
    _keepAliveWatchdog.stop();
    qDebug() << "Stopped recieveing messages on device: " << getName() << " " << getType();
    _port->close();
    emit portStateChanged(PortState::Initialized);
    onOpenSerial();
}

void SerialDevice::onSetupPort(const QSerialPortInfo& portInfo, int32_t baudRate, uint8_t dataBits, uint8_t stopBits, uint8_t parity, uint8_t flowControl) {
    _port->setBaudRate(baudRate);

    if ((dataBits <= 8) && (dataBits > 4))
        _port->setDataBits(static_cast<QSerialPort::DataBits>(dataBits));
    else {
        qWarning() << "[SerialMavlinkDevice;" << objectName() << "], can't setup port with " << dataBits << " dataBits. Using 8 as default";
        _port->setDataBits(QSerialPort::Data8);
    }

    if ((stopBits > 0) && (stopBits < 4))
        _port->setStopBits(static_cast<QSerialPort::StopBits>(stopBits));
    else {
        qWarning() << "[SerialMavlinkDevice;" << objectName() << "], can't setup port with " << stopBits << " stopBits. Using 1 as default";
        _port->setStopBits(QSerialPort::OneStop);
    }

    if ((parity != 1) && (parity < 6)) {
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
    emit portStateChanged(PortState::Initialized);
}

void SerialDevice::onReadBytes() {
    _waitPacketTimer.stop();

    QByteArray data = _port->readAll();

    if (data.size() > 0) {
        bool isSuccessfulyParsed = false;
        for (size_t i = 0; i < data.size(); ++i) {
            uint8_t byte = static_cast<uint8_t>(data[i]);
            isSuccessfulyParsed = _packetizer->onPushByte(byte);
            if(isSuccessfulyParsed) {
                _keepAliveWatchdog.start(8000);
            }
        }
        if (!isSuccessfulyParsed) {
            _waitPacketTimer.start(200);
        }
    }
}

void SerialDevice::onMessageTransmitRequest(Message msg) {
    if(_port && _port->isOpen()) {
        QByteArray data = _packetizer->packagePrepare(msg);
        _port->write(data);
    }
    else {
        _messageQueue.push_back(msg);
    }
}

void SerialDevice::setPacketizer(BasePacketizer* packetizer) {
    _packetizer = packetizer;
}