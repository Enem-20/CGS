#include "message.h"

Message::Message(const std::function<void*(void*)> copier, const std::function<void(void*)> deleter, void* msg)
    : _copier(copier)
    , _deleter(deleter)
    , _msg(msg)
{

}

Message::Message(const Message& other)
    : _copier(other._copier)
    , _deleter(other._deleter)
    , _msg(_copier(other._msg))
{}

Message::~Message() {
    if(_msg)
        _deleter(_msg);
}
