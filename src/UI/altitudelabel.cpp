#include "altitudelabel.h"

AltitudeLabel::AltitudeLabel(QWidget* parent)
    : QLabel(parent) {}

void AltitudeLabel::onAltitudeUpdated(int32_t altitude) {
    QString result("Alt: %1 m");
    setText(result.arg(altitude));
}
