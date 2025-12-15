#include "udpdevice.h"

#include <QUdpSocket>
#include <QRegularExpression>
#include <QRegularExpressionMatch>

#include "protocols/basepacketizer.h"

static QString checkIP4Addr(const QString& addr) {
    QRegularExpression re("\\b\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\b");
    QRegularExpressionMatch match = re.match(addr);
    if(!match.hasMatch()) {
        return "invalid";
    }
    return addr;
}

UDPDevice::UDPDevice(QString name, quint16 port, const QString& address, QObject *parent)
    : BaseDevice(name, "UDP", new QUdpSocket(parent), parent)
    , _selfPort(port)
    , _serverAddr(checkIP4Addr(address))
{
    _socket = qobject_cast<QUdpSocket*>(_device);
    connect(_socket, &QUdpSocket::readyRead, this, &UDPDevice::onReadBytes);
    _socket->bind(QHostAddress(address), port, QAbstractSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
}

void UDPDevice::sendRawCommand(const QByteArray& data) {
    if (_socket && _socket->isOpen() && _socket->isWritable()) {
        _socket->write(data);
    }
}

void UDPDevice::onReadBytes() {
    _waitPacketTimer.stop();
    while (_socket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(_socket->pendingDatagramSize());

        QHostAddress sender;
        quint16 senderPort;
        qint64 bytesRead = _socket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);
        _autopilotAddr = sender.toString();
        _autopilotSocket = senderPort;
        if (bytesRead > 0) {
            bool isSuccessfulyParsed = false;
            for (size_t i = 0; i < datagram.size(); ++i) {
                uint8_t byte = static_cast<uint8_t>(datagram[i]);
                isSuccessfulyParsed = _packetizer->onPushByte(byte);
            }
            if (!isSuccessfulyParsed) {
                _waitPacketTimer.start(200);
            }
        }
    }
}

void UDPDevice::onMessageTransmitRequest(Message msg) {
    if(_socket) {
        QByteArray data = _packetizer->packagePrepare(msg);
        _socket->writeDatagram(data, QHostAddress(_autopilotAddr), _autopilotSocket);
    }
    else {
        _messageQueue.push_back(msg);
    }
}
