#ifndef C_ALTITUDE_COMPONENT_H
#define C_ALTITUDE_COMPONENT_H

#include "TelemetryComponent.h"
#include "tools/tokenize.h"

class QLabel;

class AltitudeComponent : public TelemetryComponent {
    Q_OBJECT
    static uint64_t id;
    GENERATE_TYPE_FIELD(AltitudeComponent)
private:
    QLabel* _value;
public:
    explicit AltitudeComponent(QWidget* parent = nullptr);
public slots:
    void update(void* data) override;
};

#endif // C_ALTITUDE_COMPONENT_H