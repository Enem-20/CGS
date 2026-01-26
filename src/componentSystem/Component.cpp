#include "Component.h"

Component::Component(QStringView name, QWidget* parent) 
    : QWidget(parent)
{
    setObjectName(name);
}