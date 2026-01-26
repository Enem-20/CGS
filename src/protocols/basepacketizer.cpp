#include "basepacketizer.h"

QAtomicInteger<uint64_t> BasePacketizer::packetizerId = 0;

BasePacketizer::BasePacketizer(uint64_t typeHash, QObject *parent)
    : QObject{parent}
    , _typeHash(typeHash)
{
    _id = ++packetizerId;
}

uint64_t BasePacketizer::getTypeHash() const {
    return _typeHash;
}