#include "AltitudeComponent.h"

#include <QHBoxLayout>
#include <QLabel>

#include <common/mavlink.h>

uint64_t AltitudeComponent::id = 0;

AltitudeComponent::AltitudeComponent(QWidget* parent)
    : TelemetryComponent(_type + "_" + QString::number(id), parent)
{
    ++id;
    setLayout(new QHBoxLayout());
    _value = new QLabel("Altitude: %1");
    layout()->addWidget(_value);
    //_value->setObjectName("_value");
}

void AltitudeComponent::update(void* data) {
    mavlink_altitude_t* altitude = reinterpret_cast<mavlink_altitude_t*>(data);
    _value->setText(QString::number(altitude->altitude_relative));
}