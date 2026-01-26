#ifndef VEHICLE_H
#define VEHICLE_H

#include <QWidget>
#include <QHash>

#include "componentSystem/CompositeComponent.h"

class Vehicle : public CompositeComponent {
    Q_OBJECT

public:
    explicit Vehicle(QWidget* parent = nullptr);
    virtual ~Vehicle();
};

#endif // VEHICLE_H
