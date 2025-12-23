#ifndef BASEPACKETIZER_H
#define BASEPACKETIZER_H

#include <QObject>
#include <qatomic.h>

class Message;

class BasePacketizer : public QObject {
    Q_OBJECT

    static QAtomicInteger<uint64_t> packetizerId;
    uint64_t _id;

public:
    explicit BasePacketizer(QObject *parent = nullptr);

signals:
    void paramValueReceived(Message msg);
    void messageReceived(Message msg);
    void messageTransmitRequest(const QByteArray& data);
    void notSuccessfullyParsed();
    void successfullyParsed();
    
public slots:
    virtual void onSendMessageRequest(Message data) = 0;
    virtual void onPushByte(uint8_t byte) = 0;
    virtual QByteArray packagePrepare(Message msg) = 0;
};

#endif // BASEPACKETIZER_H
