#include "Parameter.h"

#include <common/mavlink.h>

Parameter::Parameter() {
    _changed.reset();
}

void Parameter::onMessage(Message msg) {
    mavlink_param_value_t paramValue;
    mavlink_message_t* converted = msg.read<mavlink_message_t>();
    mavlink_msg_param_value_decode(converted, &paramValue);
    
    char idBuffer[PARAMETER_ID_SIZE + 1]{'\0'};
    std::memcpy(idBuffer, paramValue.param_id, sizeof(char) * PARAMETER_ID_SIZE);
    _id = QString(idBuffer);
    _value = paramValue.param_value;
    _type = paramValue.param_type;
    _count = paramValue.param_count;
    _index = paramValue.param_index;

    _changed.reset();
    maskChanged(_changed.to_ullong());
}

void Parameter::onIdChanged(QString id) {
    _id = id;
    _changed.set(0, true);
    maskChanged(_changed.to_ullong());
}

void Parameter::onValueChanged(float value) {
    _value = value;
    _changed.set(1, true);
    maskChanged(_changed.to_ullong());
}

void Parameter::onTypeChanged(uint8_t type) {
    _type = type;
    _changed.set(2, true);
    maskChanged(_changed.to_ullong());
}

void Parameter::onCountChanged(uint16_t count) {
    _count = count;
    _changed.set(3, true);
    maskChanged(_changed.to_ullong());
}

void Parameter::onIndexChanged(uint16_t index) {
    _index = index;
    _changed.set(4, true);
    maskChanged(_changed.to_ullong());
}

void Parameter::onVehicleSync() {
    _changed.reset();
    maskChanged(_changed.to_ullong());
}

QString Parameter::getId() const {
    return _id;
}

float Parameter::getValue() const {
    return _value;
}

uint8_t Parameter::getType() const {
    return _type;
}

uint16_t Parameter::getCount() const {
    return _count;
}

uint16_t Parameter::getIndex() const {
    return _index;
}
