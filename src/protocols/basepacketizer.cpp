#include "basepacketizer.h"

QAtomicInteger<uint64_t> BasePacketizer::packetizerId = 0;

BasePacketizer::BasePacketizer(QObject *parent)
    : QObject{parent}
{
    _id = ++packetizerId;
}
