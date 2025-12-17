#ifndef BASEPACKETIZER_H
#define BASEPACKETIZER_H

#include <QObject>

class Message;

class BasePacketizer : public QObject {
    Q_OBJECT

public:
    explicit BasePacketizer(QObject *parent = nullptr);

signals:
    void messageReceived(Message msg);
    void messageTransmitRequest(Message msg);

public slots:
    virtual void onSendMessageRequest(Message msg) = 0;
    virtual bool onPushByte(uint8_t byte) = 0;
    virtual QByteArray packagePrepare(Message msg) = 0;
};

#endif // BASEPACKETIZER_H
