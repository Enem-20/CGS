#ifndef C_COMPONENT_H
#define C_COMPONENT_H

#include <QString>

#include <QWidget>

class Component : public QWidget {
    Q_OBJECT
public:
    explicit Component(QStringView name, QWidget* parent = nullptr);
public slots:
    virtual void update(void* data) = 0;
};

#endif // C_COMPONENT_H