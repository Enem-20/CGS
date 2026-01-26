#include "ChannelGroup.h"

uint64_t ChannelGroup::_counter = 0;

ChannelGroup::ChannelGroup(QObject* parent) 
    : _id(++_counter)
{}