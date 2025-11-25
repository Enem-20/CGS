#ifndef UDPMAVLINKDEVICE_H
#define UDPMAVLINKDEVICE_H

#include "mavlinkdevice.h"

class QUdpSocket;

class UDPMavlinkDevice : public MavlinkDevice
{
    Q_OBJECT
private:
    quint16 _selfPort;
    QString _serverAddr;
    QString _autopilotAddr;
    quint16 _autopilotSocket;
    QUdpSocket* _socket;
public:
    explicit UDPMavlinkDevice(quint16 port = 14550, const QString& address = "127.0.0.1", QObject *parent = nullptr);
    QThread* getSocketThread();
protected slots:
    virtual void readBytes() override;
public slots:
    void sendCommand(const mavlink_message_t& command);
};

#endif // UDPMAVLINKDEVICE_H
