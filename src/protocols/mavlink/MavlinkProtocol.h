#ifndef C_MAVLINK_PROTOCOL_H
#define C_MAVLINK_PROTOCOL_H

#include "../Protocol.h"

class MavlinkProtocol : public Protocol {
    Q_OBJECT
public:
    explicit MavlinkProtocol(QObject* parent = nullptr);
};

#endif // C_MAVLINK_PROTOCOL_H