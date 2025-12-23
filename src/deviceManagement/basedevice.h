#ifndef BASEDEVICE_H
#define BASEDEVICE_H

#include <QQueue>
#include <QThread>
#include <QTimer>

#include "protocols/message.h"

enum class PortState : uint8_t {
    Uninitialized,
    Initialized,
    Opened
};

class BaseDevice : public QThread {
    Q_OBJECT
private:
    QTimer _queueSendTimer;
    QTimer _connectionWatchdog;
    uint64_t _typeHash;
    bool _packageisValid = true;
protected:
    QQueue<Message> _messageQueue;
    QByteArray _inputBuffer;
    QTimer _waitPacketTimer;
    QIODevice* _device;
    QString _name;
    QString _type;
public:
    explicit BaseDevice(const QString& name, QString type, uint64_t typeHash, QIODevice* device, QObject *parent = nullptr);
    virtual ~BaseDevice();

    QStringView getName() const;
    QStringView getType() const;
    uint64_t getTypeHash() const;
signals:
    void messageReceived(Message msg);
    void portStateChanged(PortState state);
    void portCreated();
    void portInitialized();
    void portOpened();
    void portClosed();
    void receivingBegin();
    bool byteReceived(uint8_t byte);
    void receivingEnd();

protected slots:
    virtual void onReadBytes() = 0;
    virtual void onWaitPacketTimeout();
public slots:
    virtual void onSendRawCommand(const QByteArray& data) = 0;
};

#endif // BASEDEVICE_H
