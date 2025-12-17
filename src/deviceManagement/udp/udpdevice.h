#ifndef UDPDEVICE_H
#define UDPDEVICE_H

#include "../basedevice.h"

class QUdpSocket;

class UDPDevice : public BaseDevice {
    Q_OBJECT

private:
    quint16 _selfPort;
    QString _serverAddr;
    QString _autopilotAddr;
    quint16 _autopilotSocket;
    QUdpSocket* _socket;

public:
    explicit UDPDevice(QString name, quint16 port = 14550, const QString& address = "127.0.0.1", QObject *parent = nullptr);

protected:
    void sendRawCommand(const QByteArray& data) override;
public slots:
    void setPacketizer(BasePacketizer* packetizer) override;
protected slots:
    virtual void onReadBytes() override;
    void onMessageTransmitRequest(Message msg) override;
    
};

#endif // UDPDEVICE_H
