#ifndef MAVLINKPACKETIZER_H
#define MAVLINKPACKETIZER_H

#include "../basepacketizer.h"

struct __mavlink_message;
typedef __mavlink_message mavlink_message_t;

struct __mavlink_status;
typedef __mavlink_status mavlink_status_t;

class MavlinkPacketizer : public BasePacketizer {
    Q_OBJECT
    mavlink_message_t* _msg;
    mavlink_status_t* _status;

public:
    explicit MavlinkPacketizer(QObject *parent = nullptr);
    ~MavlinkPacketizer();
    
public slots:
    void onSendMessageRequest(Message msg) override;
    bool onPushByte(uint8_t byte) override;
    QByteArray packagePrepare(Message msg) override;
};

#endif // MAVLINKPACKETIZER_H
