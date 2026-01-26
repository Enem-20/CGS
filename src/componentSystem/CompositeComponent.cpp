#include "CompositeComponent.h"

#include <QHBoxLayout>

CompositeComponent::CompositeComponent(QStringView name, QWidget* parent) 
    : Component(name, parent)
{
    setLayout(new QHBoxLayout());
}

CompositeComponent::~CompositeComponent() {
    
}

void CompositeComponent::onAddComponent(Component* component) {
    Component* comp = findChild<Component*>(component->objectName());
    if(!comp) {
        layout()->addWidget(component);
        component->setParent(this);
    } else {
        qWarning() << "Component " << component->objectName() << " already exists";
    }
}