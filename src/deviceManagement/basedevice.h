#ifndef BASEDEVICE_H
#define BASEDEVICE_H

#include <QQueue>
#include <QThread>
#include <QTimer>

#include "protocols/message.h"

class BasePacketizer;

enum class PortState : uint8_t {
    Uninitialized,
    Initialized,
    Opened
};

class BaseDevice : public QThread
{
    Q_OBJECT
private:
    QTimer _queueSendTimer;
    QTimer _connectionWatchdog;
    bool _packageisValid = true;

protected:
    QQueue<Message> _messageQueue;
    QByteArray _inputBuffer;
    QTimer _waitPacketTimer;
    QIODevice* _device;
    BasePacketizer* _packetizer;
    QString _name;
    QString _type;

protected:
    virtual void sendRawCommand(const QByteArray& data) = 0;

public:
    explicit BaseDevice(const QString& name, const QString& type, QIODevice* device, QObject *parent = nullptr);
    virtual ~BaseDevice();

    QStringView getName() const;
    QStringView getType() const;
signals:
    void messageReceived(Message msg);
    void portStateChanged(PortState state);
    void portCreated();
    void portInitialized();
    void portOpened();
    void portClosed();

protected slots:
    virtual void onReadBytes() = 0;
    virtual void onWaitPacketTimeout();
public slots:
    virtual void onMessageTransmitRequest(Message msg) = 0;
    virtual void setPacketizer(BasePacketizer* packetizer) = 0;
};

#endif // BASEDEVICE_H
