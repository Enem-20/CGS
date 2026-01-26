#ifndef C_TELEMETRY_COMPONENT_H
#define C_TELEMETRY_COMPONENT_H

#include "../Component.h"

class TelemetryComponent : public Component {
    Q_OBJECT
public:
    explicit TelemetryComponent(QStringView name, QWidget* parent = nullptr);
public slots:
    virtual void update(void* data) = 0;
};

#endif // C_TELEMETRY_COMPONENT_H