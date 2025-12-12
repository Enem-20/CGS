#ifndef STATUSTEXT_H
#define STATUSTEXT_H

#include <QObject>
#include <QString>

#include "mavlink/mavlinksubscriber.h"

class StatusText : public MavlinkSubscriber {
    Q_OBJECT

public:
    explicit StatusText(QObject *parent = nullptr);

    QVector<uint32_t> getSubscribtionMessageIds() const override;

private:
    void handleMavlink(const mavlink_statustext_t& msg);

public slots:
    void onMessageReceived(const mavlink_message_t& message) override;

signals:
    void logUpdated(QString msg, QString severity);
};

#endif // STATUSTEXT_H
