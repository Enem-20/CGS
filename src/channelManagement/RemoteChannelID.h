#ifndef S_REMOTE_CHANNEL_ID
#define S_REMOTE_CHANNEL_ID

#include <cstdint>
#include <QHash>

struct RemoteChannelID {
    uint64_t _channelID;
    uint64_t _portID;
    uint64_t _portType;
    uint64_t _protocolID;
    uint64_t _sourceID;
    uint64_t _sourceType;
};

size_t qHash(const RemoteChannelID& id, size_t seed = 0) noexcept;

bool operator==(const RemoteChannelID &lhs, const RemoteChannelID &rhs) noexcept;

#endif // S_REMOTE_CHANNEL_ID