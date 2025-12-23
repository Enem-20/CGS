#ifndef UDPDEVICE_H
#define UDPDEVICE_H

#include "../basedevice.h"
#include "tools/tokenize.h"
#include "tools/hashes.h"

class QUdpSocket;

class UDPDevice : public BaseDevice {
    Q_OBJECT
    
private:
    quint16 _selfPort;
    QString _serverAddr;
    QString _autopilotAddr;
    quint16 _autopilotSocket;
    QUdpSocket* _socket;
    bool _initialized = false;

public:
    explicit UDPDevice(QString name, quint16 port = 14550, const QString& address = "127.0.0.1", QObject *parent = nullptr);

public slots:
    void onSendRawCommand(const QByteArray& data) override;

protected slots:
    virtual void onReadBytes() override;
};

#endif // UDPDEVICE_H
