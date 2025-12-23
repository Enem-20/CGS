#include "udpdevice.h"

#include <QUdpSocket>
#include <QRegularExpression>
#include <QRegularExpressionMatch>

static QString checkIP4Addr(const QString& addr) {
    QRegularExpression re("\\b\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\b");
    QRegularExpressionMatch match = re.match(addr);
    if(!match.hasMatch()) {
        return "invalid";
    }
    return addr;
}

UDPDevice::UDPDevice(QString name, quint16 port, const QString& address, QObject *parent)
    : BaseDevice(name, TOKENIZE(UDPDevice), hashes::fnv1a64(TOKENIZE(UDPDevice)), new QUdpSocket(parent), parent)
    , _selfPort(port)
    , _serverAddr(checkIP4Addr(address))
{
    _socket = qobject_cast<QUdpSocket*>(_device);
    connect(_socket, &QUdpSocket::readyRead, this, &UDPDevice::onReadBytes);
    _socket->bind(QHostAddress(address), port, QAbstractSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    _waitPacketTimer.setInterval(5000);
    connect(&_waitPacketTimer, &QTimer::timeout, this, [this](){
        _initialized = false;
        emit portClosed();
    });
}

void UDPDevice::onSendRawCommand(const QByteArray& data) {
    if (_socket && _socket->isOpen() && _socket->isWritable()) {
        _socket->writeDatagram(data, QHostAddress(_autopilotAddr), _autopilotSocket);
    }
}

void UDPDevice::onReadBytes() {
    if(!_initialized) {
        _initialized = true;
        emit portOpened();
    }

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
                isSuccessfulyParsed = emit byteReceived(byte);
            }
        }
    }
}