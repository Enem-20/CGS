#ifndef MAVLINKSUBSCRIBER_H
#define MAVLINKSUBSCRIBER_H

#include <QObject>

#include <QVector>

#include "mavlinktypes.h"

class MavlinkSubscriber : public QObject {
    Q_OBJECT

public:
    MavlinkSubscriber(QObject* parent) : QObject(parent) {}
    virtual ~MavlinkSubscriber() {}

    virtual QVector<uint32_t> getSubscribtionMessageIds() const = 0;

public slots:
    virtual void onMessageReceived(const mavlink_message_t& message) = 0;

signals:
    void sendMessage(const mavlink_message_t& message);
};

#endif // MAVLINKSUBSCRIBER_H
