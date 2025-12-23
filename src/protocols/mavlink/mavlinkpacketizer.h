#ifndef MAVLINKPACKETIZER_H
#define MAVLINKPACKETIZER_H

#include "../basepacketizer.h"
#include "MavlinkFwd.h"

class MavlinkPacketizer : public BasePacketizer {
    Q_OBJECT

    mavlink_message_t* _msg;
    mavlink_status_t* _status;

private:
    void sendEmits(uint32_t msgId, Message msg);

public:
    explicit MavlinkPacketizer(QObject *parent = nullptr);
    ~MavlinkPacketizer();

signals:
    void heartbeatReceived(Message msg);
    void paramValueReceived(Message msg);
    void logEntryReceived(Message msg);
    void logDataReceived(Message msg);
    void statusTextReceived(Message msg);
    void attitudeReceived(Message msg);
    void localPositionNEDReceived(Message msg);
    void globalPositionINTReceived(Message msg);

private slots:
    void onHeartbeat(Message msg);

public slots:
    void onSendMessageRequest(Message msg) override;
    void onPushByte(uint8_t byte) override;
    QByteArray packagePrepare(Message msg) override;
};

#endif // MAVLINKPACKETIZER_H
