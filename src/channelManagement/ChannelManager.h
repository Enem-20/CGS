#ifndef C_CHANNEL_MANAGER_H
#define C_CHANNEL_MANAGER_H

#include <QSqlDriver>
#include <QSqlDatabase>
#include <QThread>

#include "channelManagement/RemoteChannelID.h"

class Protocol;
class RemoteChannel;

class ChannelManager : public QThread {
    Q_OBJECT

    static ChannelManager* _instance;

    explicit ChannelManager(QObject* parent = nullptr);
    QSqlDatabase _db;
    QHash<uint64_t, Protocol*>  _protocols;
    QHash<RemoteChannelID, RemoteChannel*> _channels;

public:
    static ChannelManager* getInstance();
    
    RemoteChannel* getChannel(RemoteChannelID id);

signals:
    void activeChannelChanged(RemoteChannelID id);

public slots:
    void createChannel(RemoteChannelID channelId);
    void onChannelDetected(RemoteChannelID channelId);
    void onChannelLost(RemoteChannelID channelId);
};

#endif // C_CHANNEL_MANAGER_H