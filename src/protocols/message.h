#ifndef MESSAGE_H
#define MESSAGE_H

#include <functional>
#include <cstring>

struct Message
{
    std::function<void*(void*)> _copier;
    std::function<void(void*)> _deleter;
    void* _msg;

    explicit Message(const std::function<void*(void*)> copier, const std::function<void(void*)> deleter, void* msg = nullptr);
    Message(const Message& other);
    //explicit Message(Message&& other) noexcept;
    ~Message();

    template<class MsgType>
    void write(MsgType* msg, const std::function<void*(void*)> copier = [](void* msg){
        MsgType* msgTyped = reinterpret_cast<MsgType*>(msg);
        MsgType* result = nullptr;
        std::memcpy(result, msgTyped, sizeof(MsgType));
        return reinterpret_cast<void*>(result);
    }, const std::function<void(void*)> deleter = [](void* msg){delete reinterpret_cast<MsgType*>(msg);});
    template<class MsgType>
    MsgType* read();
};

template<class MsgType>
void Message::write(MsgType* msg, const std::function<void*(void*)> copier, const std::function<void(void*)> deleter) {
    _msg = reinterpret_cast<void*>(msg);
    _copier = copier;
    _deleter = deleter;
}

template<class MsgType>
MsgType* Message::read() {
    return reinterpret_cast<MsgType*>(_msg);
}

#endif // MESSAGE_H
