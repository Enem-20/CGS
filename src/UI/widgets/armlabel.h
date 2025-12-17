#ifndef ARMLABEL_H
#define ARMLABEL_H

#include <QLabel>

class ArmLabel : public QLabel {
public:
    ArmLabel(QWidget* parent = nullptr);

public slots:
    void onArmStateUpdated(bool state);
};

#endif // ARMLABEL_H
