#ifndef C_COMPOSITE_COMPONENT_H
#define C_COMPOSITE_COMPONENT_H

#include "Component.h"

class CompositeComponent : public Component {
    Q_OBJECT
public:
    explicit CompositeComponent(QStringView name, QWidget* parent = nullptr);
    ~CompositeComponent();

public slots:
    void onAddComponent(Component* component);
};

#endif // C_COMPOSITE_COMPONENT_H