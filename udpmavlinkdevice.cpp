#include "udpmavlinkdevice.h"

#include <QUdpSocket>
#include <QRegularExpression>

#include <common/mavlink.h>

static QString checkIP4Addr(const QString& addr) {
    QRegularExpression re("\\b\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\b");
    QRegularExpressionMatch match = re.match(addr);
    if(!match.hasMatch()) {
        return "invalid";
    }
    return addr;
}

UDPMavlinkDevice::UDPMavlinkDevice(quint16 port, const QString& address, QObject *parent)
    : MavlinkDevice(new QUdpSocket(parent), parent)
    , _selfPort(port)
    , _serverAddr(checkIP4Addr(address))
{
    _socket = qobject_cast<QUdpSocket*>(_device);
    connect(_socket, &QUdpSocket::readyRead, this, &UDPMavlinkDevice::readBytes);
    _socket->bind(QHostAddress(address), port, QAbstractSocket::ShareAddress | QUdpSocket::ReuseAddressHint);

    readBytes();
}

QThread* UDPMavlinkDevice::getSocketThread() {
    return _socket->thread();
}

void UDPMavlinkDevice::readBytes() {
    _waitPacketTimer.stop();
    while(_socket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(_socket->pendingDatagramSize());

        QHostAddress sender;
        quint16 senderPort;
        qint64 bytesRead = _socket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);
        _autopilotAddr = sender.toString();
        _autopilotSocket = senderPort;
        if(bytesRead > 0) {
            mavlink_message_t msg;
            mavlink_status_t status;
            bool isSuccessfulyParsed = false;
            for(size_t i = 0; i < datagram.size(); ++i) {
                uint8_t byte = static_cast<uint8_t>(datagram[i]);
                isSuccessfulyParsed = mavlink_parse_char(MAVLINK_COMM_0, byte, &msg, &status);
                if(isSuccessfulyParsed) {
                    emit messageReceived(msg);
                }
            }
            if(!isSuccessfulyParsed) {
                _waitPacketTimer.start(200);
            }
        }
    }
}

void UDPMavlinkDevice::sendCommand(const mavlink_message_t& command) {
    uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
    uint16_t len = mavlink_msg_to_send_buffer(buffer, &command);
    QByteArray data(reinterpret_cast<char*>(buffer), len);
    if(_socket) {
        _socket->writeDatagram(data, QHostAddress(_autopilotAddr), _autopilotSocket);
    }
    else
        _messageQueue.push(data);
}
