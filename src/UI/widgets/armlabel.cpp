#include "armlabel.h"

ArmLabel::ArmLabel(QWidget* parent)
    : QLabel(parent) {}

void ArmLabel::onArmStateUpdated(bool state) {
    setText(state ? "ARMED" : "DISARMED");
}
