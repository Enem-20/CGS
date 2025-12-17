#ifndef MESSAGE_H
#define MESSAGE_H

#include <functional>
#include <cstring>

struct Message {
    std::function<void*(void*)> _copier;
    std::function<void(void*)> _deleter;
    void* _msg = nullptr;

    Message() = default;
    explicit Message(const std::function<void*(void*)> copier, const std::function<void(void*)> deleter, void* msg = nullptr);
    Message(const Message& other);
    //explicit Message(Message&& other) noexcept;
    ~Message();

    template<class MsgType>
    void write(MsgType* msg, const std::function<void*(void*)> copier = [](void* msg){
        MsgType* msgTyped = reinterpret_cast<MsgType*>(msg);
        MsgType* result = new MsgType;
        *result = *msgTyped;
        return reinterpret_cast<void*>(result);
    }, 
    const std::function<void(void*)> deleter = [](void* msg){
        MsgType* typedMsg = reinterpret_cast<MsgType*>(msg);
        delete typedMsg;
    });
    template<class MsgType>
    MsgType* read();
};

template<class MsgType>
void Message::write(MsgType* msg, const std::function<void*(void*)> copier, const std::function<void(void*)> deleter) {
    MsgType* buffer = new MsgType();
    *buffer = *msg;
    _msg = reinterpret_cast<void*>(buffer);
    _copier = copier;
    _deleter = deleter;
}

template<class MsgType>
MsgType* Message::read() {
    return reinterpret_cast<MsgType*>(_msg);
}

#endif // MESSAGE_H
