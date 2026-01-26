#ifndef C_PROTOCOL_MONITOR_H
#define C_PROTOCOL_MONITOR_H

#include <QObject>
#include <QTimer>

#include "channelManagement/RemoteChannelID.h"
#include "message.h"
#include "tools/hashes.h"

class MavlinkPacketizer;
class BasePacketizer;
class BaseDevice;

class ProtocolMonitor : public QObject {
    Q_OBJECT

    MavlinkPacketizer*  _mavlinkPacketizer;
    BaseDevice*         _device;

    QHash<RemoteChannelID, QTimer*> _detectedChannelsTimers;

public:
    explicit ProtocolMonitor(QObject* parent = nullptr);
    ~ProtocolMonitor();

    void init();
signals:
    void channelDetected(RemoteChannelID id);
    void channelLost(RemoteChannelID id);
    
    bool checkContainsBlockable(RemoteChannelID id);
    void createChannel(RemoteChannelID id);
    void packetizerUpdate(BasePacketizer* packetizer);

private slots:
    void onHeartbeatMessage(Message msg);
    void onChannelLost(RemoteChannelID id);
};

#endif // C_PROTOCOL_MONITOR_H