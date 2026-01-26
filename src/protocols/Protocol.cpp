#include "Protocol.h"

#include "ProtocolFabric.h"

Protocol* Protocol::_instance = nullptr;

Protocol::Protocol(uint64_t typeHash, ProtocolFabric* fabric, QObject* parent) 
    : QObject(parent)
    , _fabric(fabric)
    , _typeHash(typeHash)
{}

uint64_t Protocol::getTypeHash() const {
    return _typeHash;
}

ProtocolFabric* Protocol::getFactory() {
    return _fabric;
}