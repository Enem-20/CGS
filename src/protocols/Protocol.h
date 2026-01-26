#ifndef C_PROTOCOL_H
#define C_PROTOCOL_H

#include <cstdint>

#include <QObject>

class ProtocolFabric;

class Protocol : public QObject {
    Q_OBJECT
    static Protocol* _instance;
    ProtocolFabric* _fabric;
    uint64_t _typeHash;
public:
    explicit Protocol(uint64_t _typeHash, ProtocolFabric* fabric, QObject* parent = nullptr);
    template<class Derived = Protocol, class ... Args>
    static Protocol* getInstance(Args ... args) {
        if(_instance) [[likely]]
            return _instance;
        return _instance = new Derived(args...);
    }
public slots:
    uint64_t getTypeHash() const;
    ProtocolFabric* getFactory();
};

#endif // C_PROTOCOL_H