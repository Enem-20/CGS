#ifndef C_CHANNEL_GROUP_H
#define C_CHANNEL_GROUP_H

#include <QObject>

class ChannelGroup : public QObject {
    Q_OBJECT
private:
    static uint64_t _counter;

    uint64_t _id;
public:
    explicit ChannelGroup(QObject* parent = nullptr);
};

#endif // C_CHANNEL_GROUP_H