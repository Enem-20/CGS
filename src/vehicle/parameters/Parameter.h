#ifndef C_PARAMETER_H
#define C_PARAMETER_H

#include <bitset>

#include "protocols/ProtocolSubscriber.h"

class Parameter : public ProtocolSubscriber {
    Q_OBJECT
public:
    static constexpr size_t PARAMETER_ID_SIZE = 16;
private:
    QString _id;
    float _value;
    uint8_t _type;
    uint16_t _count;
    uint16_t _index;

    std::bitset<5> _changed;
public:
    Parameter();
signals:
    void maskChanged(size_t parameterMask);
public slots:
    void onMessage(Message msg) override;

    void onIdChanged(QString id);
    void onValueChanged(float value);
    void onTypeChanged(uint8_t type);
    void onCountChanged(uint16_t count);
    void onIndexChanged(uint16_t index);

    void onVehicleSync();

    QString getId() const;
    float getValue() const;
    uint8_t getType() const;
    uint16_t getCount() const;
    uint16_t getIndex() const;
};

#endif // C_PARAMETER_H