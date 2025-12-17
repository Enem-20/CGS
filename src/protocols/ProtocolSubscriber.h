#ifndef C_PROTOCOL_SUBSCRIBER_H
#define C_PROTOCOL_SUBSCRIBER_H

#include <QObject>

#include "message.h"

class ProtocolSubscriber : public QObject {
    Q_OBJECT
public:
    explicit ProtocolSubscriber(QObject* parent = nullptr);
signals:
    void sendMessageRequest(Message msg);
public slots:
    virtual void onMessage(Message msg) = 0;
};

#endif // C_PROTOCOL_SUBSCRIBER_H